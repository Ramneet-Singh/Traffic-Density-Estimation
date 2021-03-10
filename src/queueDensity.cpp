#include <densityEstimation.hpp>

using namespace cv;
using namespace std;

pair<vector<float>, vector<float>> queueDensity(string videoPath)
{
	VideoCapture cap(videoPath);
	if (cap.isOpened() == false)
	{
		cout << "Cannot open the video file" << endl;
		cin.get();
	}
	Mat im_src1 = imread("resources/background.png");
	Mat im_src(328, 778, CV_8UC1);
	cv::cvtColor(im_src1, im_src, cv::COLOR_BGR2GRAY);
	Size size(328, 778);
	Mat im_dst1 = Mat::zeros(size, CV_8UC1);
	Mat im_dst2 = Mat::zeros(size, CV_8UC1);

	vector<Point2f> pts_dst;
	pts_dst.push_back(Point2f(0, 0));
	pts_dst.push_back(Point2f(size.width, 0));
	pts_dst.push_back(Point2f(size.width, size.height));
	pts_dst.push_back(Point2f(0, size.height));

	vector<Point2f> datap;
	datap.push_back(Point2f(1000, 220));
	datap.push_back(Point2f(1280, 220));
	datap.push_back(Point2f(1550, 1078));
	datap.push_back(Point2f(440, 1078));

	Mat tform = findHomography(datap, pts_dst);
	warpPerspective(im_src, im_dst1, tform, size);
	long int frameNum = 0;
	vector<float> timestamps, queueDenValues;
	while (true)
	{
		frameNum++;
		timestamps.push_back((frameNum / 15.0));
		Mat frame;
		bool bSuccess = cap.read(frame);

		if (bSuccess == false)
		{
			cout << "Found the end of the video" << endl;
			break;
		}
		Mat im_2(328, 778, CV_8UC1);
		cv::cvtColor(frame, im_2, cv::COLOR_BGR2GRAY);
		warpPerspective(im_2, im_dst2, tform, size);
		Mat temp = im_dst2 - im_dst1;
		Mat out = Mat::zeros(size, CV_8UC1);
		float count = 0;
		for (int y = 0; y < 778; y++)
		{
			for (int x = 0; x < 328; x++)
			{
				if ((int)(temp.at<uint8_t>(y, x) >= 14))
				{
					(temp.at<uint8_t>(y, x)) = 255;
					count++;
				}
				else
				{
					(temp.at<uint8_t>(y, x)) = 0;
				}
			}
		}
		count = count / (328 * 600);
		queueDenValues.push_back(count);

		imshow("vid", temp);
		if (waitKey(10) == 27)
		{
			cout << "Esc key is pressed by user. Stopping the video" << endl;
			break;
		}
	}
	return make_pair(timestamps, queueDenValues);
}