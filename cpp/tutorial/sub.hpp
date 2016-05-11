#include <iostream>
#include "opencv2/opencv.hpp"
#include <string>
#include <vector>

using namespace std;
using namespace cv;

void command_to_imagenames(vector<string> &result, char* input[], int params_number)
{
    for(int i=0; i< params_number; i++)
    {
    string temp;
    temp = string(input[i]);
    result.push_back(temp);
    }
}

class Image
{
    private:
        string name;
        Mat image;

    public:
           

    private:
    

    public:
        Image(string to_imagefile);
        void show();
};

Image::Image(string to_imagefile)
{
    name = to_imagefile;
    image = imread(to_imagefile,-1);
}

void Image::show()
{
    imshow(name,image);
}
