#include "calibration.hpp"


int main()
{

	const int data_num = 11;
	const int a = 5;

	string filename[data_num];
	get_filename(filename,data_num);
	int i;

	//Mat view[data_num];
	//Mat gray[data_num];

	vector<Mat> view;
	vector<Mat> gray;
	vector<Mat> temp;

	Image calib[data_num];

	for(i = 0; i <data_num ; i++)
	{
		calib[i].init(filename[i]);
	}

	for(i = 0; i <data_num ; i++)
	{
		view.push_back(calib[i].color());
	}

	cv::Size patternsize(7,6); // 中心の個数
	vector<Point2f> centers; // 検出された中心が格納されます
	vector<vector<Point2f>> imagePoints;
 
	int count = 0;

 	for(i = 0; i < data_num; i++)
 	{
		bool patternfound = findCirclesGrid(view[i], patternsize, centers);
		
		if(patternfound)
		{	
			gray.push_back(calib[i].gray());
			temp.push_back(calib[i].color());
			cornerSubPix( gray[count], centers, Size(12,12),
             	Size(-1,-1), TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

			imagePoints.push_back(centers);
			//drawChessboardCorners( view[i], patternsize, Mat(centers), patternfound );
			count++;
		}
	}



	Mat cameraMatrix;
	Mat distCoeffs;
	vector<Mat> rotationVectors;
	vector<Mat> translationVectors;

	vector<Point3f> objectPoints;

	for(int y=0; y<6; ++y)
	{
    	for(int x=0; x<7; ++x)
    	{
        	objectPoints.push_back(cv::Point3f(x*a,y*a,0));
    	}
	}

	vector<vector<Point3f>> arrayObjectPoints;
	for(int n=0; n < count; n++)
	{
    	arrayObjectPoints.push_back(objectPoints);
	}

	//cout << arrayObjectPoints.size() << endl;
	//cout << imagePoints.size() << endl;

	double reprojection_Error = calibrateCamera(		
                    arrayObjectPoints,      //input     // 特徴点の世界座標系における座標値
                    imagePoints,            //input objectPointsに対応する特徴点の画像上での座標値
                    view[0].size(),              // 画像サイズ
                    cameraMatrix,           // 入出力用のカメラ内部パラメータ行列
                    distCoeffs,             // 歪み係数の出力ベクトル
                    rotationVectors,        // 各ビューにおいて推定された回転ベクトル
                    translationVectors      // 各ビューにおいて推定された並進ベクトル
                    );

	//cout << cameraMatrix << endl;
	//cout << distCoeffs << endl;
	//cout << rotationVectors.size() << endl;
	//cout << translationVectors.size() << endl;
	cout << "Reprojection Error:" << reprojection_Error << endl;

	Mat result[count];

	for(i=0;i<count;i++)
	{
		undistort(temp[i], result[i], cameraMatrix, distCoeffs);
		namedWindow("result" + to_string(i), WINDOW_NORMAL);
		imshow("result" + to_string(i), result[i]);
		imwrite("result" + to_string(i) + ".JPG", result[i]);
	}
	waitKey(0);
}