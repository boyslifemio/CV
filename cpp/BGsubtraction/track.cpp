
#include "sub.hpp"

int main(int argc, char* argv[]){
  int count = 0, camera = 0, user_mode = 0;
  user_mode = mode();
  Mat frame, tempMask, output;
  getImage(camera, count, frame);
  vector<Mask> maskArray(12, Mask(frame.rows, frame.cols));
  Ptr<BackgroundSubtractor> MOG2model=createBackgroundSubtractorMOG2();

	while (waitKey(1) == -1){
    if(count==0){
      Mat frame, tempMask, output;
      Ptr<BackgroundSubtractor> MOG2model=createBackgroundSubtractorMOG2();
      cout << "----- init done -----" << endl;
    }
    getImage(camera, count, frame);
	  if (frame.empty()){
      break;
    }
    calcBackground(MOG2model, frame, output, tempMask, user_mode);
    plusflags(maskArray[camera].flagArray, tempMask);
    count++;
    if(count==1800){
      finalOperate(maskArray[camera],camera);
      cout << "next camera -> n" << endl << "stop operation -> ESC" << endl;
      int k = waitKey(0);
      if(k == 'n'){
        count = 0;
        camera++;
      }
      else if(k == 27){
        break;
      }
      if(camera == 12) break;
      cout << "fin " + to_string(camera) + "th camera remap" << endl;
      destroyAllWindows();
    }
	}
  destroyAllWindows();
}
