#include "file.hpp"

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
	int a = 10;
	Image picture(file); //= file;
	Mat output = picture.gaussian();
	namedWindow("Sekiya-Naru",WINDOW_AUTOSIZE);
	imshow("Sekiya-Naru",output);

	waitKey(0);
}
