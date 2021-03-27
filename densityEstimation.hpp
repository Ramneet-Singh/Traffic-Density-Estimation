#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <thread>
#include <pthread.h>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

struct spatialParallelThreadArgs
{
    int threadId;
    cv::Mat bgFrameBlk;
    cv::Mat curFrameBlk;
};

cv::Mat getBackgroundFrame(cv::Mat transformMat);
cv::Mat computeHomography1(int X, int Y);
float computeQueueDensity(cv::Mat &currentFrame, cv::Mat &backgroundFrame);
void subSampledQueueDensity(std::string videoPath, int numSkip);
void reducedResolutionQueueDensity(std::string videoPath, int X, int Y);
void outputSpatialParallelQueueDensity(std::string videoPath, int numSplits);
void *spatialParallelThreadFunc(void *arg);
cv::Mat getFrameBlk(cv::Mat &frame, int startCoordinate, int blockSize);

cv::Mat computeHomography();
void computeDynamicDensity(cv::Mat &prev, cv::Mat &nxt, float &dynVar);
void outputQueueAndDynamic(std::string videoPath);