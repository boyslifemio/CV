#include <iostream>
#include "opencv2/opencv.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>

#define CAMERA_NUMBER 12

using namespace std;
using namespace cv;

class Mask{
  public:
    Mask(int height, int width);
    Mask(Mat& splited_hconcat);
    void finalizeMask(int threshold, int camera);
    void plusflags(Mat& tempMask);
    Mat return_flagArray();
    int height,width;
  private:
    void drawMask(int threshold);
    Mat finalizedMask, flagArray;
};

Mask::Mask(int input_height, int input_width){
  height = input_height;
  width = input_width;
  flagArray = Mat::zeros(height, width, CV_32S);
  finalizedMask = Mat::zeros(height, width, CV_8U);
}

Mask::Mask(Mat& splited_hconcat){
  flagArray = splited_hconcat;
  finalizedMask = Mat::zeros(flagArray.rows, flagArray.cols, CV_8U);
}

Mat Mask::return_flagArray() {return flagArray;}

void Mask::plusflags(Mat& tempMask){
  for(int i=0; i<tempMask.rows; i++){
    for(int j=0; j<tempMask.cols; j++){
      if(tempMask.at<unsigned char>(i,j) == 0)
        flagArray.at<int>(i,j) += 1;  
    }
  }
}

void Mask::drawMask(int threshold){
  finalizedMask = Mat::zeros(flagArray.rows, flagArray.cols, CV_8U); //閾値が変わった時マスクを初期化
  for(int i=0;i<finalizedMask.rows;i++){
    for(int j=0;j<finalizedMask.cols;j++){
      if(flagArray.at<int>(i,j) <= threshold){
        finalizedMask.at<unsigned char>(i,j) = 255;  
      }
    }
  }
}

void Mask::finalizeMask(int threshold, int camera){
  int k = 0;
  cout << "increase threshold -> i" << endl << "decrease threshold -> d" << endl << "finalize a mask -> f" << endl; 
  while(1){
    //cout << threshold << endl;
    drawMask(threshold);
    imshow("finalized-mask",finalizedMask);
    k = waitKey(20);
    if(k=='i') threshold+=10;
    else if(k=='d') threshold-=10;
    else if(k=='f') break;
  }
  imwrite(to_string(camera) + ".png", finalizedMask);
  destroyAllWindows();
}
int mode(){
  string k;
  cout << "Preview mode -> yes" << endl << "No Preview mode -> no" << endl << "-> ";
  cin >> k;
  if(k=="yes") return 1;
  else if(k=="no") return 0;
  else return 0;
}

Mat getImage(char* data_place[], int count){
  Mat input, concat, src; 
  for(int camera = 0; camera < 12; camera++){
    if (camera == 0) {
      ostringstream scount,scamera;
      scamera << setfill('0') << setw(2) << camera;  
      scount << setfill('0') << setw(4) << count;  
      src = imread(string(data_place[1]) + scount.str() + "-" + scamera.str() + ".png", 1); 
      camera++;
    }
    ostringstream scount,scamera;
    scamera << setfill('0') << setw(2) << camera;  
    scount << setfill('0') << setw(4) << count;  
    input = imread(string(data_place[1]) + scount.str() + "-" + scamera.str() + ".png", 1);
    vconcat(src, input, concat);
    src = concat;
  }
  return concat;
}

void calcBackground(Ptr<BackgroundSubtractor>& MOG2model,Mat& frame, Mat& output, Mat& tempMask, int mode){
  MOG2model -> apply(frame, tempMask);
  bitwise_and(frame, frame, output, tempMask);
	if(mode==1){
    imshow("output", output);
    imshow("tempmask", tempMask);
    waitKey(2);
  }
}

/*
void plusflags(vector<vector<unsigned int>>& flagArray, Mat& tempMask){
  for(int i=0;i<tempMask.rows;i++){
    for(int j=0;j<tempMask.cols;j++){
      if(tempMask.at<unsigned int>(i,j)==0)
        flagArray[i][j]+=1;
    }
  }
}
*/

void finalOperate(Mask& mask,int camera){
  unsigned int threshold = 1000;
  cout << "input threshold(defalut:1000, Max:3300) >>";
  cin >> threshold;
  mask.finalizeMask(threshold, camera);
}

void split_concat_mask(Mask source, vector<Mask>& output){
  for(int i=0;i<CAMERA_NUMBER;i++){
    Mat temp_FlagArray(source.return_flagArray(), Rect(0, (source.height/12)*i, source.width, source.height/12));
    output.push_back(Mask(temp_FlagArray));
  }
}

void MakeMask(char* place[], vector<Mat>& MaskArray){ 
  int count = 0, end = 0;
  int show = mode();
  cout << "input end-frame index -> ";
  cin >> end;
  Ptr<BackgroundSubtractor> MOG2model=createBackgroundSubtractorMOG2();
  Mat concat_frame = getImage(place, count); //各フレームで12カメラの画像を縦方向につなげた画像
  Mask concat_mask(concat_frame.rows, concat_frame.cols);
  cout << "init done, start BGST" << endl;
  while(count < end){
    Mat output, tempMask;
    calcBackground(MOG2model, concat_frame, output, tempMask, show);
    concat_mask.plusflags(tempMask);
    concat_frame = getImage(place, count); //各フレームで12カメラの画像を縦方向につなげた画像
    count++;
  }
  destroyAllWindows();
  vector<Mask> each_camera_mask;
  split_concat_mask(concat_mask, each_camera_mask);
  count = 0;
  for (Mask m : each_camera_mask){
    finalOperate(m, count);
    count++;
  }
  destroyAllWindows();
}
