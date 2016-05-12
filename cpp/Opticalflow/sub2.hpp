#include <iostream>
#include "opencv2/opencv.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cmath>

using namespace std;
using namespace cv;

void command_to_imagenames(vector<string> &result, char* number_of_images_char, int* number_of_images)
{
    *number_of_images = atoi(number_of_images_char);
    string number_of_images_str = number_of_images_char;
    for(int i=0; i< *number_of_images; i++)
    {
        string filename;
        ostringstream sout;
        sout << setfill('0') << setw(4) << i;
        string temp = sout.str();
        filename ="camera1-1/" +  temp + "-01.png";
        result.push_back(filename);
    }
}

void showOpticalFlow(const Mat& flow, Mat& opticalflow)
{
    opticalflow = Mat::zeros(flow.rows, flow.cols, CV_64F);
    int flow_ch = flow.channels();
    int opt_ch = opticalflow.channels();
    float dx,dy,df;
    for(int i=0;i<flow.rows;i++){
      float* src_p = (float*)(flow.data+flow.step*i);
      float* dst_p = (float*)(opticalflow.data+opticalflow.step*i);
      for(int j=0;j<flow.cols;j++){
        dx=src_p[0];
        dy=src_p[1];
        df=sqrt(pow(dx,2.0)+pow(dy,2.0));
        //cout << df << endl;
        opticalflow.at<double>(i,j)=df;
        src_p+=flow_ch;
        dst_p+=opt_ch;
      }
    }
}

void calculateSum(Mat& sum, vector<Mat>& flow){
  float div;
  for(int h=0;h<flow.size();h++){
    for(int i=0;i<flow[h].rows;i++){
      for(int j=0;j<flow[h].cols;j++){
        div=flow[h].at<double>(i,j);
        sum.at<double>(i,j)+=div;
      }
    }
  }
}

void createMask(Mat& mask, Mat& sum){
  double average=0;
  for(int i=0;i<sum.rows;i++){
    for(int j=0;j<sum.cols;j++){
      average+=(double)(sum.at<double>(i,j)/(sum.rows*sum.cols));
    }
  }
  cout << average << endl;
  for(int i=0;i<sum.rows;i++){
    for(int j=0;j<sum.cols;j++){
      if(sum.at<double>(i,j)>average/2){
        mask.at<Vec3b>(i,j)[0]=255;
        mask.at<Vec3b>(i,j)[1]=255;
        mask.at<Vec3b>(i,j)[2]=255;
      }
    }
  }
}

class Frame
{
    public:
        Mat color_frame;
        Mat gray_frame;

    public:
        Frame(Mat input);
};

Frame::Frame(Mat input)
{
    color_frame = input;
    cvtColor(color_frame, gray_frame,CV_RGB2GRAY);
}

class Image
{
    private:
        string name;

    public:
        Mat image;   
        Mat grey_image;
    private:
    

    public:
        Image(string to_imagefile);
        void show();
};

Image::Image(string to_imagefile)
{
    name = to_imagefile;
    image = imread(to_imagefile,-1);
    grey_image = imread(to_imagefile, 0);
}

void Image::show()
{
    imshow(name,image);
}
