#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Size boardSize = Size(6, 7)


Class Calibration{
  public:
    void Calibrate();
    void save();
    Calibration(int height, int weight);
  private:
    vector<Mat> 


};

Calibration::Calibration(){


}


void Calibration::Calibrate(Mat inputFrame){
  Mat grayFrame;
  cvtColor(inputFrame, greyFrame, CV_BGR2GRAY);

  cout << "calibrated" << endl;
}

void showUsage(){
  cout << "Press q to save calibration.yaml and finish this program" << endl;
  cout << "Press s to save a image for calibration, you need to save 4 images at least" << endl;
  cout << "Press c to calibration , then saved images in cache are deleted" << endl;
}

void deleteCache(int* frameNum, vector<Point3f>* objectPoints, vector<Point3f>* imagePoints){
  *frameNum = 0;
  objectPoints->clear();
  imagePoints->clear();
  cout << "Cache deleted" << endl;
}

int main(int argc, char* argv[]){
  if(argc != 2){
    cout << "./this.out <port-number>" << endl;
    return -1;
  }
  VideoCapture cap;
  cap.open(int(*argv[1]-'0'));
  if(!cap.isOpened()) return -1;

	int frameWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frameHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << frameHeight << frameWidth << endl;

  Calibration C(frameHeight, frameWeight);

  vector<Point3f> obj;
	for(int j=0; j<boardSize.height; j++)
    for(int i=0; i<boardSize.width; i++)
      obj.push_back(Point3f(i%boardSize.width, j%boardSize.height, 0));
	vector<vector<Point3f>> objectPoints;
	vector<Point2f> oneFramePoints;
	vector<vector<Point2f>> imagePoints;
  showUsage();
  int frameNum = 0;
  while(1){
    int key = waitKey(5);
    Mat inputFrame;
    cap >> inputFrame;

    bool found = findCirclesGrid(inputFrame, boardSize, oneFramePoints);
    if (found) drawChessboardCorners(inputFrame, boardSize, Mat(points), found);
    imshow("captured-image", inputFrame);

    if(key == 113){ //q
      break;
    }
    else if(key == 115 && found){ //s
      frameNum++;
      //cornerSubPix(InputFrame, oneFramePoints, frameHeight, fmameWeight)
      imagePoints.push_back(oneFramePoints);
      objectPoints.push_back(obj);
      cout << frameNum << " frames saved!" << endl;
    }
    else if(key == 99){ //c
      Calibration.calibrate(objectPoints, imagePoints); 
      deleteCache(&frameNum, &objectPoints, &imagePoints);
    }
  }
  Calibration.save();
  destroyAllWindows();
  return 0;
}
