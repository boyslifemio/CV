#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>


using namespace cv;
using namespace std;

class Image
{
	private:
		string image_path;
		Mat image;

	public:
		Image();
		void init(string temp);
		Mat color();
		Mat gray();
};

Image::Image()
{

}

void Image::init(string temp)
{
	string image_path = temp;
	image = imread(image_path, IMREAD_COLOR);
	if(image.empty())
	{
		cout << "miss" << endl;
		exit(-1);
	}
}

Mat Image::color()
{
	return image;
}

Mat Image::gray()
{
	Mat gray_image;
	cvtColor(image, gray_image,CV_RGB2GRAY);
	return gray_image;
}

string* get_filename(string* names, int number)
{
	int a = 0;
	for(a = 0; a<number;a++)
	{
		names[a] = "./" + to_string(a) + ".JPG";
	}
	return names;
}