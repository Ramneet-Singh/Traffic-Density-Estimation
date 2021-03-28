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

#define ROWNUM 778
#define COLNUM 328

struct spatialParallelThreadArgs
{
    int threadId;
    cv::Mat bgFrameBlk;
    cv::Mat curFrameBlk;
};

struct temporalParallelThreadArgs
{
    int threadId;
    int numThreads;
    std::string videoPath;
};

cv::Mat getBackgroundFrame(cv::Mat transformMat);
float computeQueueDensity(cv::Mat &currentFrame, cv::Mat &backgroundFrame);
void outputSpatialParallelQueueDensity(std::string videoPath, int numSplits);
void *spatialParallelThreadFunc(void *arg);
cv::Mat getFrameBlk(cv::Mat &frame, int startCoordinate, int blockSize);

void outputTemporalParallelQueueDensity(std::string videoPath, int numSplits);
void combineParallelOutFiles(int numThreads, std::string outputDir, bool spatial);
void *temporalParallelThreadFunc(void *arg);

cv::Mat computeHomography();
void computeDynamicDensity(cv::Mat &prev, cv::Mat &nxt, float &dynVar);
void outputQueueAndDynamic(std::string videoPath);