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
#include <opencv2/core/utils/filesystem.hpp>

#define ROWNUM 778
#define COLNUM 328

// ============================METHOD 1 DECLARATIONS==========================

void subSampledQueueDensity(std::string videoPath, int numSkip);

// ============================METHOD 2 DECLARATIONS==========================

void reducedResolutionQueueDensity(std::string videoPath, int X, int Y);

// ============================METHOD 3 DECLARATIONS==========================

struct spatialParallelThreadArgs
{
    int threadId;
    cv::Mat bgFrameBlk;
    cv::Mat curFrameBlk;
};

void outputSpatialParallelQueueDensity(std::string videoPath, int numSplits);
void *spatialParallelThreadFunc(void *arg);
cv::Mat getFrameBlk(cv::Mat &frame, int startCoordinate, int blockSize);

// ============================METHOD 4 DECLARATIONS==========================

struct temporalParallelThreadArgs
{
    int threadId;
    int numThreads;
    std::string videoPath;
};

void outputTemporalParallelQueueDensity(std::string videoPath, int numSplits);
void *temporalParallelThreadFunc(void *arg);

// ============================SUBTASK2 DECLARATIONS==========================

cv::Mat getBackgroundFrame(cv::Mat transformMat);
float computeQueueDensity(cv::Mat &currentFrame, cv::Mat &backgroundFrame);
cv::Mat computeHomography(int X = 1920, int Y = 1080);

void computeDynamicDensity(cv::Mat &prev, cv::Mat &nxt, float &dynVar);
void outputQueueAndDynamic(std::string videoPath);