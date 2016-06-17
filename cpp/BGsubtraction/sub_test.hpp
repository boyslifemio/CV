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
    void finalizeMask(int threshold, int camera);
    void plusflags(Mat& tempMask);
    Mat return_flagArray();
    int height,width;
    //vector<vector<unsigned int>> flagArray;
  private:
    void giveMask(Mat& finalizedMask,Mat& flagArray, int threshold);
    Mat finalizedMask, flagArray;
};

Mask::Mask(int input_height, int input_width){
  height = input_height;
  width = input_width;
  flagArray = Mat::zeros(height, width, CV_8U);
  finalizedMask = Mat::zeros(height, width, CV_8U);
}

Mat Mask::return_flagArray() {return flagArray;}

void Mask::plusflags(Mat& tempMask){
  for(int i=0; i<tempMask.rows; i++){
    for(int j=0; j<tempMask.cols; j++){
      if(tempMask.at<unsigned char>(i,j) == 0)
        flagArray.at<unsigned char>(i,j) += 1;  
        //flagArray[i][j] += 1;
    }
  }
}
void Mask::giveMask(Mat& finalizedMask,Mat& flagArray,int threshold){
  finalizedMask = Mat::zeros(flagArray.rows, flagArray.cols, CV_8U);
  for(int i=0;i<finalizedMask.rows;i++){
    for(int j=0;j<finalizedMask.cols;j++){
      if(flagArray.at<unsigned char>(i,j) <= threshold){
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
    giveMask(finalizedMask, flagArray, threshold);
    imshow("finalized-mask",finalizedMask);
    k = waitKey(20);
    if(k=='i') threshold+=30;
    else if(k=='d') threshold-=30;
    else if(k=='f') break;
  }
  imwrite(to_string(camera) + ".png", finalizedMask);
}

int mode(){
  string k;
  cout << "Preview mode -> yes" << endl << "No Preview mode -> no" << endl << "-> ";
  cin >> k;
  if(k=="yes") return 1;
  else if(k=="no") return 0;
  else return 0;
}

void getImage(char* data_place[], int count, Mat& hconcat_frame){
  Mat input;
  vector<Mat> src_images;
  for(int camera = 0; camera < 12; camera++){
    ostringstream scount,scamera;
    scamera << setfill('0') << setw(2) << camera;  
    scount << setfill('0') << setw(4) << count;  
    input = imread(string(data_place[1]) + "/"  + scount.str() + "-" + scamera.str() + ".png", 1);
    src_images.push_back(input);
  }
  hconcat(src_images, src_images.size(), hconcat_frame);
}

void calcBackground(Ptr<BackgroundSubtractor>& MOG2model,Mat& frame, Mat& output, Mat& tempMask){
  MOG2model->apply(frame, tempMask);
  bitwise_and(frame, frame, output, tempMask);
	if(false){
    imshow("output", output);
    imshow("tempmask", tempMask);
  }
}

void plusflags(vector<vector<unsigned int>>& flagArray, Mat& tempMask){
  for(int i=0;i<tempMask.rows;i++){
    for(int j=0;j<tempMask.cols;j++){
      if(tempMask.at<unsigned char>(i,j)==0)
        flagArray[i][j]+=1;
    }
  }
}

void finalOperate(Mask& maskInfo,int camera){
  unsigned int threshold = 1000;
  cout << "input threshold(defalut:1000, Max:3300) >>";
  cin >> threshold;
  maskInfo.finalizeMask(threshold, camera);
}

void split_hconcat_mask(Mask source, vector<Mask>& output){
  for(int i=0;i<CAMERA_NUMBER;i++){
    Mat temp_FlagArray(source.return_flagArray(), Rect(0, (source.height/12)*i, source.width, source.height / 12));
  }
}

void MakeMask(char* params[], vector<Mat>& MaskArray){ 
  int count = 0, end = 0;
  Mat hconcat_frame;
  cout << "input end-frame index -> ";
  cin >> end;
  Ptr<BackgroundSubtractor> MOG2model=createBackgroundSubtractorMOG2();
  getImage(params, count, hconcat_frame); //各フレームで12カメラの画像を縦方向につなげた画像
  Mask hconcat_mask(hconcat_frame.rows, hconcat_frame.cols);
  while(count < end){
    Mat output, tempMask;
    calcBackground(MOG2model, hconcat_frame, output, tempMask);
    hconcat_mask.plusflags(tempMask);
    getImage(params, count, hconcat_frame); //各フレームで12カメラの画像を縦方向につなげた画像
    count++;
  }
  vector<Mask> each_camera_mask;
  split_hconcat_mask(hconcat_mask, each_camera_mask);
}
