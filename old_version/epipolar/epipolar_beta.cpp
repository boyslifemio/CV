#include "epipolar.hpp"


int main()
{
	vector<Mat> images;
	int i,j;

	string path;

	for(i = 0; i < 9; i++)
	{
		path = "./" + to_string(i) + ".JPG";
		Image input(path);
		images.push_back(input.Get_Image());
	}


	Ptr<AKAZE> detector = AKAZE::create();

  vector<vector<KeyPoint>> keys;
	vector<Mat> descriptors;

	vector<KeyPoint> key;
	Mat descriptor;

	for(i = 0; i < 9; i++)
	{
		detector->detectAndCompute(images[i], noArray(), key, descriptor);
		keys.push_back(key);
		descriptors.push_back(descriptor);
	}
	//keysには全ての画像の特徴点、descriptorsには全ての画像の特徴点の特徴量
	//Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");

	BFMatcher matcher(NORM_HAMMING, true); //matcher生成

//基準は5.jpgの画像
	int center = 4;
	Mat epipolar_center_desc = descriptors[center];
	vector<KeyPoint> epipolar_center_key = keys[center];
	vector<vector<KeyPoint>> around_key;

	vector<DMatch> matches;
	vector<vector<DMatch>> cross_image_matches; //[10]


//5.jpg1と他の画像のmatch情報をcross_image_matchesベクトルに格納
	for(i=0; i<9;i++ )
	{
		if(i != center)
		{
			matcher.match(epipolar_center_desc,descriptors[i],matches);
			cross_image_matches.push_back(matches);
		}
		around_key.push_back(keys[i]);
	}

//良いmatchだけを残す
//center_good_keysには各indexに他の画像との一致点座標
//around_good_keysには各indexにそれぞれの画像の、基準画像との一致点座標
	vector<vector<DMatch>> cross_good_Matches;//[cross_image_matches.size()];
	vector<vector<KeyPoint>> center_good_keys;//[cross_image_matches.size()];
	vector<vector<KeyPoint>> around_good_keys;//[cross_image_matches.size()];
	vector<DMatch> temp1;
	vector<KeyPoint> temp2;
	vector<KeyPoint> temp3;

	for(j=0;j<cross_image_matches.size() ; j++)
	{
		//cout << "j" << j << endl;

		for(i = 0; i < cross_image_matches[j].size(); i++)
		{
			//cout << i << endl;
			//cout << cross_image_matches[j][i].distance  << endl;
			if(cross_image_matches[j][i].distance < 33)
			{
				//cout << "ok" << i << endl;
			  temp1.push_back(cross_image_matches[j][i]);
			  temp2.push_back(epipolar_center_key[cross_image_matches[j][i].queryIdx]);
		  	temp3.push_back(around_key[j][cross_image_matches[j][i].trainIdx]);
			}
		}
		cross_good_Matches.push_back(temp1);
		center_good_keys.push_back(temp2);
		around_good_keys.push_back(temp3);
	}


	vector<Point2f> points1, points2;
	vector<vector<Point2f>> center_points, around_points;

	for(i=0;i<cross_image_matches.size();i++)
	{
		KeyPoint::convert(center_good_keys[i],points1);
		KeyPoint::convert(around_good_keys[i],points2);
		center_points.push_back(points1);
		around_points.push_back(points2);
	}
	//
	// vector<Point2f> points1, points2;
	// KeyPoint::convert(good_keys1,points1);
	// KeyPoint::convert(good_keys2,points2);


	vector<Mat> Homographies;
	vector<Mat> fundamental;
	vector<Mat> essenntail;
	Mat homo;
	Mat fund;
	Mat esse;
	for(i=0;i<cross_image_matches.size();i++)
	{
		homo = findHomography(center_points[i],around_points[i],CV_RANSAC,10);
		fund = findFundamentalMat(center_points[i],around_points[i],FM_RANSAC,2);
		esse = findEssentialMat(center_points[i],around_points[i],1.0,Point2d(0, 0),RANSAC,0.999,2.0);

		Homographies.push_back(homo);
		fundamental.push_back(fund);
		essenntail.push_back(esse);
	}

	vector<Vec3f> line1;

	computeCorrespondEpilines(center_points[3],1,fundamental[3],line1);

	cout << center_points[3].size() << endl;

	float a, b, c;
	int k = 0;
	for(auto it = line1.begin(); it != line1.end(); ++it)
	{
		a = (*it)[0];
		b = (*it)[1];
		c = (*it)[2];
		line(images[4], Point(0,-c/b),Point(images[4].cols-1, -(a/b*images[4].cols+c/b)),Scalar::all(255));
		k++;
	}
cout << k <<endl;

namedWindow("result",WINDOW_NORMAL);
imshow("result",images[4]);
cvSetMouseCallback("result",on_mouse(images,fundamental));
waitKey(0);
}
