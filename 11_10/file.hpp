#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>


using namespace cv;
using namespace std;

class FileName
{

};

class Image
{
	private:
		String image_path;
		Mat naru;

	public:
		//string name;
		Image(string str);
		//void Show(Mat s);
		Mat cat_image();
		Mat gaussian();

	//void gausu();
};

Image::Image(string str)
{
	image_path = "./" + str;
	cout << "Try to open " + image_path + "\n"<< endl;
	naru = imread(image_path, IMREAD_COLOR);
	if(naru.empty())
	{
		cout << "Failed to open\n" << endl;
		exit(-1);
	}
}

/*
Mat Image::cat_image()
{
	//Mat naru = imread(image_path, IMREAD_COLOR);
	return naru;
}
*/

Mat Image::gaussian()
{
	GaussianBlur(naru,naru,Size(3,3),0);
	return naru;
}

