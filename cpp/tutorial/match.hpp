#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>


using namespace cv;
using namespace std;

typedef struct MouseInfo
{
    int event;
    int  flag;
    unsigned int  x;
    unsigned int  y;
} MouseInfo;

class Image
{
    private:
        vector<KeyPoint> this_keys;
        Mat this_descriptor;
    public:
        Mat original_image;
        Mat grey_image;
    private:
    public:
        Image(Mat input_image);
        void detect_and_compute();
        void give_keys(vector<KeyPoint> &keys);
        void give_descriptor(Mat &descriptor);
};

Image::Image(Mat input_image)
{
    original_image = input_image;
    grey_image = grey_image;
}

void Image::detect_and_compute()
{
    Ptr<AKAZE> detector = AKAZE::create();
    detector->detectAndCompute(original_image, noArray(), this_keys, this_descriptor);
}

void Image::give_keys(vector<KeyPoint> &keys)
{
    keys = this_keys;
}

void Image::give_descriptor(Mat &descriptor)
{
    descriptor = this_descriptor;
}

class TrackInfo{
    private:
    public:
        Mat descriptor;
        vector<KeyPoint> point;
    private:

    public:
        TrackInfo(Mat start_image, vector<KeyPoint> track_key);
        TrackInfo(Mat next_image);
};

TrackInfo::TrackInfo(Mat start_image, vector<KeyPoint> track_key)
{
    point.push_back(track_key[0]);
    Ptr<AKAZE> detector = AKAZE::create();
    detector->detectAndCompute(start_image, noArray(), point, descriptor);
}

TrackInfo::TrackInfo(Mat next_image)
{
    Ptr<AKAZE> detector = AKAZE::create();
    detector->detectAndCompute(next_image, noArray(), point, descriptor);
}

void mouse_callback( int event, int x, int y, int flags, void *param )
{
	if ( event == EVENT_LBUTTONDOWN )
	{
        MouseInfo *mouse = (MouseInfo*)param;
        mouse->flag = flags;
        mouse->event = event;
        mouse->x = x;
        mouse->y = y;
        //cout << "[" << mouse->x << "," << mouse->y << "]" << endl;
	}
	if ( event == EVENT_LBUTTONUP )
	{
        MouseInfo *mouse = (MouseInfo*)param;
        mouse->flag = flags;
        mouse->event = event;
        mouse->x = x;
        mouse->y = y;
        //cout << "[" << mouse->x << "," << mouse->y << "]" << endl;
	}
}
