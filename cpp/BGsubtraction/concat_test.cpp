#include"sub_test.hpp"


using namespace std;
using namespace cv;

int main(int argc, char* argv[]){
  if(argc != 2) cout << "usage: ./test.out data-place/";
  int count = 0, end = 0;
  Mat hconcat_frame;
  cout << "input end-frame index -> ";
  cin >> end;
  Ptr<BackgroundSubtractor> MOG2model=createBackgroundSubtractorMOG2();
  while(count < end){
    Mat output, tempMask;
    getImage(argv, count, hconcat_frame); //各フレームで12カメラの画像を縦方向につなげた画像
    Mask hconcat_mask(hconcat_frame.rows, hconcat_frame.cols);
    calcBackground(MOG2model, hconcat_frame, output, tempMask);
    hconcat_mask.plusflags(tempMask);
    count++;
  }
}
