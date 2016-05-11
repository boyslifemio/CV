#include <iostream>
#include <opencv2/opencv.hpp>
//#include <opencv2/stitching.hpp>
#include <vector>
#include <string>


using namespace cv;
using namespace std;

class Image
{
  public:
    Mat input;
     Image(string path);
     Mat Get_Pic();
     Mat descriptors;
     vector<KeyPoint> keys;
};

Image::Image(string path)
{
  input = imread(path, IMREAD_COLOR);
  Ptr<AKAZE> detector = AKAZE::create();
  detector->detectAndCompute(input, noArray(), keys, descriptors);
}

Mat Image::Get_Pic()
{
  return input;
}


Mat find_goodMatch()
{

}
