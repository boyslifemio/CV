#include <iostream>
#include "opencv2/opencv.hpp"
#include <string>
#include <vector>
#include <opencv2/superres/superres.hpp>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "sub2.hpp"

using namespace std;
using namespace cv;
using namespace cv::cuda;

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        cout << "usage: ./this.out [number of input images]"<< endl;
        exit(0);
    }
    
    int number_of_images;
    vector<string> image_names;
    command_to_imagenames(image_names, argv[1], &number_of_images);

    vector<Image> images;

    for(auto n: image_names)
    {
        cout << n << ", ";
    }

    for(int i=0;i<number_of_images;i++)
    {
        images.push_back(Image(image_names[i]));
    }
    
    Mat previous, current;
    previous = images[0].grey_image;

    Mat flow(previous.cols, previous.rows, CV_32FC2);
    Mat optical_flow,Sumflow;
    Mat mask = Mat::zeros(images[0].image.rows,images[0].image.cols,CV_8U);
    vector<Mat> flowArray;
    Ptr<DenseOpticalFlow> opticalFlow = createOptFlow_DualTVL1();
    int count = 0;
    cout << endl << "-----computeing flow-----" << endl;
    while(count < number_of_images-1)
    {
        current = images[count+1].grey_image;
        opticalFlow->calc(previous, current, flow);
        showOpticalFlow(flow, optical_flow);
        flowArray.push_back(optical_flow);
        previous = current;
        count++;
        cout << "fin-> " << count << endl;
    }
    cout << "-----fin to calculate flow-----" << endl;
    Sumflow = Mat::zeros(flow.rows, flow.cols, CV_64F);
    calculateSum(Sumflow, flowArray);
    cout << "-----create mask-----" << endl;
    createMask(mask, Sumflow);
    imwrite("mask.png",mask);
    return 0;
}
