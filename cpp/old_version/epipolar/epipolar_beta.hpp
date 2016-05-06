#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <vector>
#include <string>


using namespace cv;
using namespace std;

class Image
{
  private:
    Mat input;
  public:
     Image(string path);
     Mat Get_Image();
};

Image::Image(string path)
{
  input = imread(path, IMREAD_COLOR);
}

Mat Image::Get_Image()
{
  return input;
}
//void *param=NULL
void on_mouse(int event, int x, int y, int flags, vector<Mat>* images,vector<Mat>* fundamental)
{
  vector<Vec3f> line1;
  Point2f pt;
  pt.x = x;
  pt.y = y;
  computeCorrespondEpilines(pt,1,fundamental[3],line1);
  float a, b, c;fundamental;
  for(auto it = line1.begin(); it != line1.end(); ++it)
  {
    a = (*it)[0];
    b = (*it)[1];
    c = (*it)[2];
    line(images[0], Point(0,-c/b),Point(images[4].cols-1, -(a/b*images[4].cols+c/b)),Scalar::all(255));

    namedWindow("result1",WINDOW_NORMAL);
    imshow("result1",images[0]);
    waitKey(0);
  }
}
