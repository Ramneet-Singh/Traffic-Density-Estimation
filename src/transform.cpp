#include <angleCorrection.hpp>

using namespace cv;
using namespace std;

Mat transform(string &img_name, Mat &src_img, vector<Point2f> &src_vec)
{
    Size size(328, 778);
    Mat dst_img = Mat::zeros(size, CV_8UC3);
    vector<Point2f> dst_vec;

    dst_vec.push_back(Point2f(0, 0));
    dst_vec.push_back(Point2f(size.width - 1, 0));
    dst_vec.push_back(Point2f(size.width - 1, size.height - 1));
    dst_vec.push_back(Point2f(0, size.height - 1));

    Mat H = findHomography(src_vec, dst_vec);
    warpPerspective(src_img, dst_img, H, size);

    return H;
}
