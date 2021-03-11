#include <densityEstimation.hpp>

using namespace cv;
using namespace std;

Mat getBackgroundFrame(Mat transformMat)
{
	Mat im_src1 = imread("resources/background.png");
	Mat im_src(328, 778, CV_8UC1);
	cv::cvtColor(im_src1, im_src, cv::COLOR_BGR2GRAY);
	Size size(328, 778);
	Mat im_dst1 = Mat::zeros(size, CV_8UC1);
	warpPerspective(im_src, im_dst1, transformMat, size);

	return im_dst1;
}

float computeQueueDensity(Mat &currentFrame, Mat &backgroundFrame)
{
	Mat temp = currentFrame - backgroundFrame;
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
	count = count / (328 * 300);

	return count;
}
