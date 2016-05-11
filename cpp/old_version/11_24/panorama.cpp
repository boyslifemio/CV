#include "panorama.hpp"


int main()
{
	vector<Mat> input;
	Mat panorama;

	Mat image1 = imread("1.JPG",IMREAD_UNCHANGED );
	Mat image2 = imread("2.JPG",IMREAD_UNCHANGED );
	Mat image3 = imread("3.JPG",IMREAD_UNCHANGED );

	input.push_back(image1);
	input.push_back(image2);
	input.push_back(image3);

	Stitcher stitcher = Stitcher::createDefault();
	stitcher.stitch(input, panorama);

	cv::imwrite("panorama.jpg", panorama);
	cv::namedWindow("panorama",WINDOW_NORMAL);
	cv::imshow("panorama", panorama);
	waitKey(0);
}