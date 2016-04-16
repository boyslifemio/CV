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

	public:
		//string name;
		Image(string str);

		void Show(Mat s);
		Mat cat_image();
		Mat gaussian();

	//void gausu();
};

Image::Image(string str)
{
	image_path = "./" + str;
	cout << image_path << endl;
	//cout << "constructor" << endl;
}
void Image::Show(Mat s/*String title*/)
{
	static int i = 0;
	i++;
	//cout << name << endl;
	string num = to_string(i);
	cout << "show" << endl;
	Mat naru = s;
	namedWindow("Sekiya-Naru" + num,WINDOW_AUTOSIZE);//title;
	imshow("Sekiya-Naru" + num,naru);
}

Mat Image::cat_image()
{
	Mat naru = imread(image_path, IMREAD_COLOR);
	return naru;
}

Mat Image::gaussian()
{
	Mat naru = imread(image_path, IMREAD_COLOR);
	GaussianBlur(naru,naru,Size(3,3),0);
	return naru;
}

/*
void image::gausu()
{

}
*/

int main()
{
	string file; 
	cout << "input filename" << endl;
	cin >> file;
	//cout << file << endl;
	int a = 10;
	Image picture = file;
	//picture.cat_image();
	Mat hoge = picture.gaussian();
	Mat fuga = picture.cat_image();
	picture.Show(hoge);
	picture.Show(fuga);

	waitKey(0);
}