#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Size boardSize = Size(5, 5);

class Calibration{
  public:
    Calibration(int height, int weight);
    void calibrate(Mat* inputFrame);
    Mat calibrate2(Mat inputFrame);
    void calculate(vector<vector<Point3f>>& objectPoints, vector<vector<Point2f>>& imagePoints);
    void save();
    int getCalNum();
  private:
    int frameHeight, frameWidth;
    vector<Mat> kArray, distArray;
    int calNum;

};

Calibration::Calibration(int height, int width){
  frameHeight = height; 
  frameWidth = width; 
  calNum = 0;
}

void Calibration::calibrate(Mat* inputFrame){
  for(int i = 0; i < calNum ;i++){
    cout << kArray[i] << distArray[i] << endl;
    undistort(*inputFrame, *inputFrame, kArray[i], distArray[i]);
  }
}

Mat Calibration::calibrate2(Mat inputFrame){
  Mat output;
  for(int i = 0; i < calNum ;i++){
    undistort(inputFrame, output, kArray[i], distArray[i]);
  }
  return output;
}

void Calibration::calculate(vector<vector<Point3f>>& objectPoints, vector<vector<Point2f>>& imagePoints){
  Mat k, dist;
  vector<Mat> rvecs, tvecs;
  calibrateCamera(objectPoints, imagePoints, Size(frameWidth, frameHeight), k, dist, rvecs, tvecs);
  kArray.push_back(k);
  distArray.push_back(dist);
  calNum++;
  cout << "calculated!" << endl;
}

int Calibration::getCalNum(){
  return calNum;
}

void Calibration::save(){

}

void showUsage(){
  cout << "Press q to save calibration.yaml and finish this program" << endl;
  cout << "Press s to save a image for calibration, you need to save 4 images at least" << endl;
  cout << "Press c to calibration , then saved images in cache are deleted" << endl;
}

void deleteCache(int* frameNum, vector<vector<Point3f>>* objectPoints, vector<vector<Point2f>>* imagePoints){
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

  Calibration C(frameHeight, frameWidth);

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
    Mat inputFrame, imageForView;
    cap >> inputFrame;

    if(C.getCalNum() > 0) inputFrame = C.calibrate2(inputFrame);

    bool found = findCirclesGrid(inputFrame, boardSize, oneFramePoints);
    if (found) drawChessboardCorners(inputFrame, boardSize, Mat(oneFramePoints), found);
    resize(inputFrame, imageForView, Size(frameWidth/2, frameHeight/2), 0, 0, 0);
    namedWindow("captured-image", CV_WINDOW_NORMAL);
    imshow("captured-image", imageForView);

    if(key == 113){ //q
      break;
    }
    else if(key == 115 && found){ //s
      frameNum++;
      Mat gray;
      cvtColor(inputFrame, gray, CV_BGR2GRAY);
      cornerSubPix(gray, oneFramePoints, boardSize, Size(frameWidth, frameHeight),
          TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
      imagePoints.push_back(oneFramePoints);
      objectPoints.push_back(obj);
      cout << frameNum << " frames saved!" << endl;
    }
    else if(key == 99 && frameNum > 4){ //c
      C.calculate(objectPoints, imagePoints); 
      deleteCache(&frameNum, &objectPoints, &imagePoints);
    }
  }
  C.save();
  destroyAllWindows();
  return 0;
}
