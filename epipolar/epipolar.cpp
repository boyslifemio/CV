#include "epipolar.hpp"




int main()
{
	//vector<Mat> images;
	int i,j;

	string path1,path2;

	int a,b;
	cout << "input number>" << endl;
	cin >> a;
	cout << "input number>" << endl;
	cin >> b;


	path1 = "./" + to_string(a) + ".JPG";
	Image input1(path1);
	images.push_back(input1.Get_Image());

	path2 = "./" + to_string(b) + ".JPG";
	Image input2(path2);
	images.push_back(input2.Get_Image());

	Ptr<AKAZE> detector = AKAZE::create();

  vector<vector<KeyPoint>> keys;
	vector<Mat> descriptors;

	vector<KeyPoint> key;
	Mat descriptor;

	for(i = 0; i < 2; i++)
	{
		detector->detectAndCompute(images[i], noArray(), key, descriptor);
		keys.push_back(key);
		descriptors.push_back(descriptor);
	}
	//keysには全ての画像の特徴点、descriptorsには全ての画像の特徴点の特徴量
	//Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");

	BFMatcher matcher(NORM_HAMMING, true); //matcher生成
	vector<DMatch> matches;
	matcher.match(descriptors[0],descriptors[1],matches);

//良いmatchだけを残す
vector<DMatch> good_matches;
vector<vector<KeyPoint>> good_keys(2);


	for(i = 0; i < matches.size(); i++)
	{
		if(matches[i].distance < 40)
		{
		  good_matches.push_back(matches[i]);
		  good_keys[0].push_back(keys[0][matches[i].queryIdx]);
	  	good_keys[1].push_back(keys[1][matches[i].trainIdx]);
		}
	}

	vector<Point2f> points1, points2;
	KeyPoint::convert(good_keys[0],points1);
	KeyPoint::convert(good_keys[1],points2);

	Mat Homographies;
	//Mat fundamental;
	Mat essentail;

	Homographies = findHomography(points1,points2,CV_RANSAC,10);
	fundamental = findFundamentalMat(points1,points2,FM_RANSAC,2);
	essentail = findEssentialMat(points1,points2,1.0,Point2d(0, 0),RANSAC,0.999,2.0);

// 	vector<Vec3f> line1;
//
// 	computeCorrespondEpilines(points1,1,fundamental,line1);
//
// 	float c, d, e;
// 	int k = 0;
// 	for(auto it = line1.begin(); it != line1.end(); ++it)
// 	{
// 		c = (*it)[0];
// 		d = (*it)[1];
// 		e = (*it)[2];
// 		//line(images[1], Point(0,-e/d),Point(images[1].cols-1, -(c/d*images[1].cols+e/d)),Scalar::all(255));
// 		k++;
// 	}
//
// cout << k <<endl;

namedWindow("result",WINDOW_NORMAL);
imshow("result",images[0]);
cvSetMouseCallback("result",on_mouse);
waitKey(0);
}
