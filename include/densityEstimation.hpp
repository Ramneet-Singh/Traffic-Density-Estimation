#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

std::pair<std::vector<float>, std::vector<float>> queueDensity(std::string videoPath);

std::vector<float> dynamicDenVals(std::string videoPath);