#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <vector>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

using namespace cv;
using namespace std;

typedef struct MouseInfo{
    int event;
    int  flag;
    unsigned int  x;
    unsigned int  y;
    string title;
    Mat center;
    Mat F;
} MouseInfo;

class Image{
  public:
    String name;
    Mat image;
    Mat descriptor;
    vector<KeyPoint> kps;
    Image(string input,Ptr<AKAZE>& akaze);
    void calcKpandDesc(Ptr<AKAZE>& akaze);
    void writeLine(vector<Vec3f>& line);
    void showImage();
};

Image::Image(string input,Ptr<AKAZE>& akaze){
  name = input;
  image = imread(name, IMREAD_COLOR);
  calcKpandDesc(akaze);
}

void Image::calcKpandDesc(Ptr<AKAZE>& akaze){
  akaze -> detectAndCompute(image, noArray(), kps, descriptor);
}

void Image::writeLine(vector<Vec3f>& epipolarLine){
  float a, b, c;
  for(auto v = epipolarLine.begin(); v != epipolarLine.end(); ++v){
    a = (*v)[0];
    b = (*v)[1];
    c = (*v)[2];
    line(image, Point(0,-c/b),Point(image.cols-1, -(a/b*image.cols+c/b)),Scalar::all(255));
  }
}
/*
void Image::on_mouse(int event, int x, int y, int flags, void* param){
  MouseInfo *mInfo = (MouseInfo*)param;
  if (event == 1){
    (*mInfo).x = x;
    (*mInfo).y = y;
  }
  cout << event << endl;
}
*/
void Image::showImage(){
  imshow(name,image);
}

void calcLine(MouseInfo* info,vector<Vec3f>& epipolarLine){
  vector<Point2f> pt(1);
  pt[0].x = info->x;
  pt[0].y = info->y;
  computeCorrespondEpilines(pt, 1, info->F, epipolarLine);
}

void writeLine(MouseInfo* info, vector<Vec3f>& epipolarLine){
  float a, b, c;
  for(auto v = epipolarLine.begin(); v != epipolarLine.end(); ++v){
    a = (*v)[0];
    b = (*v)[1];
    c = (*v)[2];
    line(info->center, Point(0,-c/b),Point((info->center).cols-1, -(a/b*(info->center).cols+c/b)),Scalar::all(255));
    imshow(info->title,info->center);
  }
}

void on_mouse(int event, int x, int y, int flags, void* param){
  if (event == 1){
    vector<Vec3f> epipolarLine;
    MouseInfo *mInfo = (MouseInfo*)param;
    (*mInfo).x = x;
    (*mInfo).y = y;
    
    calcLine(mInfo, epipolarLine);
    writeLine(mInfo, epipolarLine);
  }
  cout << event << endl;
}

