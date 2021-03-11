#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

cv::Mat getBackgroundFrame(cv::Mat transformMat);
float computeQueueDensity(cv::Mat &currentFrame, cv::Mat &backgroundFrame);
cv::Mat computeHomography();
void computeDynamicDensity(cv::Mat &prev, cv::Mat &nxt, float &dynVar);
void outputQueueAndDynamic(std::string videoPath);