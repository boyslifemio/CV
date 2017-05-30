#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
 
using namespace std;

int main(int argc, char *argv[]){

  cv::namedWindow("pentagon", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
 
  cv::Mat paper = cv::Mat::zeros(cv::Size(500,500), CV_8U);
  cv::Point pt[5]; 

  pt[0] = cv::Point(20, 250);
  pt[1] = cv::Point(150, 450);
  pt[4] = cv::Point(150, 50);
  pt[3] = cv::Point(400, 100);
  pt[2] = cv::Point(400, 400);

  // InputMat, pt_array, pt_number, lineColor
  cv::fillConvexPoly( paper, pt, 5, cv::Scalar(200) ); 
   
  cv::imshow("paper", paper);

  cout << "press a key" << endl;
  while(1){
    if(cv::waitKey(30) >= 0){
      break;
    }
  }
}
