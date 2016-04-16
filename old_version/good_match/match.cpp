#include "match.hpp"


int main()
{
	vector<Image> input;
	Mat result;

	Image pic_a("4.JPG");
	Image pic_b("5.JPG");
	Image pic_c("6.JPG");

	input.push_back(pic_a);
	input.push_back(pic_b);
	input.push_back(pic_c);

	//cout << Homography << endl;

	//Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
	vector<vector<DMatch>> matches;
	BFMatcher matcher(NORM_L2, false);
	matcher.knnMatch(input[0].descriptors,input[1].descriptors,matches,2);
	vector<DMatch> temp;
	vector<vector<DMatch>> Good_Matches;
	vector<KeyPoint> query_keys, train_keys;

	int i = 0;

	for(i = 0;i < matches.size(); i++)
	{
		if(matches[i][0].distance / matches[i][1].distance < 100)
		{
			if(matches[i][0].distance < 100)
			{
				temp.push_back(matches[i][0]);
				query_keys.push_back(input[0].keys[matches[i][0].queryIdx]);
				train_keys.push_back(input[1].keys[matches[i][0].trainIdx]);
			}
		}
	}

	for(i = 0; i < query_keys.size(); i++)
	{
		cout << query_keys[i].pt << endl;
		cout << train_keys[i].pt << endl;
		cout << temp[i].distance << endl;
	}

	Mat resultimage;

	drawMatches(input[0].input,)
	// int i;
	// for(i = 0; i < matches.size(); i++)
	// {
	// 	if(matches[i].distance < 18)
	// 	{
	// 		Good_Matches.push_back(matches[i]);
	// 		good_keys1.push_back(keys1[matches[i].queryIdx]);
	// 	 	good_keys2.push_back(keys2[matches[i].trainIdx]);
	// 	}
	// }
	//
	//
	// imwrite("knn_match.jpg", dst_image);
	// namedWindow("knn",WINDOW_NORMAL);
	// imshow("knn", dst_image);
	// waitKey(0);
}
