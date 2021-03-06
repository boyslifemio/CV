#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <vector>
#include <string>


using namespace cv;
using namespace std;

vector<Mat> images;
Mat fundamental;

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
void on_mouse(int event, int x, int y, int flags,void* param )
{
  Mat temp = images[1];
  if(event == EVENT_LBUTTONDOWN)
  {
  vector<Vec3f> line1;
  vector<Point2f> pt(1);
  pt[0].x = x;
  pt[0].y = y;
  computeCorrespondEpilines(pt,1,fundamental,line1);
  float a, b, c;
  for(auto it = line1.begin(); it != line1.end(); ++it)
    {
      a = (*it)[0];
      b = (*it)[1];
      c = (*it)[2];
      //temp == image[0]
      line(images[1], Point(0,-c/b),Point(images[1].cols-1, -(a/b*images[1].cols+c/b)),Scalar::all(255));
      namedWindow("mouse",WINDOW_NORMAL);
      imshow("mouse",images[1]);
    //waitKey(0);0 to 4,
    }
  }
}
