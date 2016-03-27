#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>


#include "match.hpp"

using namespace cv;
using namespace std;

int main()
{
    VideoCapture cap(0);

    if(!cap.isOpened())
    {
        cout << " camera open failed" << endl;
        return -1;
    }

    Mat frame;
    vector<Image> images;
    int count = 0;
    int click = 0;
    Mat previous_descriptor, current_descriptor;
    vector<KeyPoint> previous_key, current_keys, current_match_key;

    int track_x, track_y;
    vector<KeyPoint> track_keypoint;
    KeyPoint begin;
    Mat track_descriptor;

    for(int i = 0; i < 5; i++)
    {   
        cap >> frame;
    }

    MouseInfo mouse;
    namedWindow("camera");
    setMouseCallback("camera", mouse_callback, &mouse);

    vector<TrackInfo> Tracking;

    Ptr<AKAZE> detector = AKAZE::create();
    vector<vector<DMatch>> matches;
    DMatch best_match;
	BFMatcher matcher(NORM_L2, false);
    double closest = 1000.0;

    while(1)
    {
        closest = 1000.0;
        cap >> frame;

        if(click == 1)
        {
            vector<KeyPoint> track_keypoint;
            detector->detectAndCompute(frame, noArray(), current_keys, current_descriptor);
	        matcher.knnMatch(previous_descriptor,current_descriptor,matches,2);
//マッチングは得たから、次はどのマッチングが１番信頼できるか定めて次の点を求める。
            
            for(int i = 0; i < matches.size(); i++)
            {
                for(int j = 0; j < matches[i].size(); j++)
                {
                    if(matches[i][j].distance < closest)
                    {
                        closest = matches[i][j].distance;
                        best_match = matches[i][j];
                    }
                }
            }

            cout << "distance>>>>>" << best_match.distance << endl;
            cout << "trainid>>>>>" << best_match.trainIdx << endl;
            cout << "next>>>>>" << current_keys[best_match.trainIdx].pt << endl;
            cout << "fin one loop" << endl;

            track_keypoint.push_back(begin);
            //cout <<"match"<< matches.size()<< endl;
        }
        //for(auto n: current_keys)
        //{
           // for(auto m: n)
            //{
          //      cout << n.pt << endl;
           // }
        //}
        //cout << current_descriptor << endl;

        imshow("camera",frame);

        if(mouse.event==1 && click == 0)
        {
            begin.pt.x = static_cast<float>(mouse.x);
            begin.pt.y = static_cast<float>(mouse.y);
            track_keypoint.push_back(begin);
            click = 1;
        }
        if(click == 1)
        {
            Tracking.push_back(TrackInfo(frame, track_keypoint));
            previous_descriptor = Tracking[count].descriptor;
            previous_key = Tracking[count].point;
            count++;
            //cout <<"under"<< count << endl;
        }

        int key = waitKey(30);
    }
    destroyAllWindows();
}
