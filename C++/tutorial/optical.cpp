#include <iostream>
#include "opencv2/opencv.hpp"
#include <string>
#include <vector>

#include "sub.hpp"

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        cout << "usage: ./this.out [image1] [image2]"  << endl;
        exit(0);
    }
    
    vector<string> image_names;
    command_to_imagenames(image_names, argv, argc);

    vector<Image> images;

//    for(auto n: image_names)
//    {
//        cout << n << endl;
//    }

    for(int i=1;i<image_names.size();i++)
    {
        images.push_back(Image(image_names[i]));
        images[i-1].show();
    }
    
    waitKey(0);
    return 0;
}
