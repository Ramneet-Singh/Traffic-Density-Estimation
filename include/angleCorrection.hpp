#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>

struct userdata
{
    cv::Mat im;
    std::vector<cv::Point2f> points;
};

void mouseHandler(int event, int x, int y, int flags, void *data_ptr);

void perspectiveCorrection(std::string img_name);

void transform(std::string &img_name, cv::Mat &im_src, std::vector<cv::Point2f> &pts_src);