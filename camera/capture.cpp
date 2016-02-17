#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

int main()
{
    cv::VideoCapture cap(0);//デバイスのオープン
    //cap.open(0);//こっちでも良い．

    if(!cap.isOpened())//カメラデバイスが正常にオープンしたか確認．
    {
        //読み込みに失敗したときの処理
        return -1;
    }

    while(1)//無限ループ
    {
        cv::Mat frame;
        cap >> frame; // get a new frame from camera

        //
        //取得したフレーム画像に対して，クレースケール変換や2値化などの処理を書き込む．
        //
        cv::namedWindow( "Window");
        cv::imshow("window", frame);//画像を表示．

        int key = cv::waitKey(1);
        cout << key << endl;
        if(key == 113)//qボタンが押されたとき
        {
            break;//whileループから抜ける．
        }
        else if(key == 115)//sが押されたとき
        {
            //フレーム画像を保存する．
            imwrite("img.png", frame);
        }
    }
    cv::destroyAllWindows();
    return 0;
}
