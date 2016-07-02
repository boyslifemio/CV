#include <iostream>
#include "opencv2/opencv.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>

using namespace std;
using namespace cv;

class Mask{
  public:
    vector<vector<unsigned int>> flagArray;
    Mask(int height, int width);
    void finalizeMask(int threshold, int camera);
  private:
    void giveMask(Mat& finalizedMask,vector<vector<unsigned int>> flagArray, int threshold);
    Mat finalizedMask;
};

Mask::Mask(int height, int width){
  flagArray.resize(height,vector<unsigned int>(width, 0)); 
  finalizedMask = Mat::zeros(height, width, CV_8U);
}

void Mask::giveMask(Mat& finalizedMask,vector<vector<unsigned int>> flagArray,int threshold){
  finalizedMask = Mat::zeros(flagArray.size(), flagArray[0].size(), CV_8U);
  for(int i=0;i<finalizedMask.rows;i++){
    for(int j=0;j<finalizedMask.cols;j++){
      if(flagArray[i][j]<threshold){
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

void getImage(int camera, int count, Mat& frame){
  ostringstream scount,scamera;
  scount << setfill('0') << setw(4) << count;  
  scamera << setfill('0') << setw(2) << camera;  
 //cout << "/home/geome-r/data/Drone_remap/chiba/" + scount.str() + "-" + scamera.str() + ".png" << endl;
  frame = imread("/home/geome-r/data/Drone_remap/remap1/" + scount.str() + "-" + scamera.str() + ".png",1);
}

void calcBackground(Ptr<BackgroundSubtractor>& MOG2model,Mat& frame, Mat& output, Mat& tempMask, int mode){
  MOG2model->apply(frame, tempMask);
  bitwise_and(frame, frame, output, tempMask);
	if(mode==1){
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


