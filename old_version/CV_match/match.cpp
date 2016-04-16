//#include <opencv2/core.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include "opencv2/features2d.hpp"
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/features2d/features2d.hpp>
#include <opencv2/opencv.hpp>
//#include <opencv/highgui.h>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <stdio.h>
#include <iostream>
#include <vector>


int main(int argc, const char* argv[])
{
    //読み込む画像のパス
  cv::String scene1_path = "./naru.png";
  cv::String scene2_path = "./naru2.jpg";

  cv::Mat dst1(cv::Size(490, 660), CV_8UC3, cv::Scalar(0, 0, 0));
  cv::Mat dst2(cv::Size(490, 660), CV_8UC3, cv::Scalar(0, 0, 0));
  //書き出す画像のパス
  //cv::String scene_12_path = "./result.jpg";


  //比較用画像を読み込む (アルファチャンネル非対応のため、IMREAD_COLORで強制する)
  cv::Mat scene1 = cv::imread(scene1_path, cv::IMREAD_COLOR);
  cv::Mat scene2 = cv::imread(scene2_path, cv::IMREAD_COLOR);



  cv::resize(scene1, dst1, dst1.size(), 0, 0, 0);
  cv::resize(scene2, dst2, dst2.size(), 0, 0, 0);



  cv::Ptr<cv::AKAZE> detector = cv::AKAZE::create();
  //cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create("ORB");
  std::vector<cv::KeyPoint> keys1;
  std::vector<cv::KeyPoint> keys2;

  detector -> detect(scene1,keys1);
  detector -> detect(scene2,keys2);

  cv::Mat result1;
  cv::Mat result2;

  result1 = dst1.clone();
  result2 = dst2.clone();

 

  for(int i = 0; i < keys1.size(); i++) 
    {
      
    cv::KeyPoint *point = &(keys1[i]);
    cv::Point center;
    int radius;
    center.x = cvRound(point->pt.x);
    center.y = cvRound(point->pt.y);
    radius = cvRound(point->size*0.25);
        
    cv::circle(result1, center, radius, cv::Scalar(255,255,0));
    }

      

  for(int i = 0; i < keys2.size(); i++) 
    {
      
    cv::KeyPoint *point = &(keys2[i]);
    cv::Point center;
    int radius;
    center.x = cvRound(point->pt.x);
    center.y = cvRound(point->pt.y);
    radius = cvRound(point->size*0.25);
        
    cv::circle(result2, center, radius, cv::Scalar(255,255,0));
    }

  cv::Mat descriptor1, descriptor2;
  detector -> compute(scene1,keys1,descriptor1);
  detector -> compute(scene2,keys2,descriptor2);

  cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce");

  std::vector<cv::DMatch> match, match12, match21;
  matcher->match(descriptor1, descriptor2, match12);
  matcher->match(descriptor2, descriptor1, match21);

  cv::Mat HIT;
  cv::drawMatches(result1, keys1, result2, keys2, match12, HIT);

  cv::namedWindow("naru1", cv::WINDOW_AUTOSIZE);
  cv::imshow("naru1",result1);
  cv::imwrite("./map_naru1.png",result1);

  cv::namedWindow("naru2", cv::WINDOW_AUTOSIZE);
  cv::imshow("naru2",result2);
  cv::imwrite("./map_naru2.png",result2);

  cv::namedWindow("hit", cv::WINDOW_AUTOSIZE);
  cv::imshow("hit",result2);
  cv::imwrite("./match.png",HIT);


  cv::waitKey(0);

  return 0;

}
