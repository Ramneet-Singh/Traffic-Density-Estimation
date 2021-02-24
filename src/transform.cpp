#include <angleCorrection.hpp>

using namespace cv;
using namespace std;

void transform(string &img_name, Mat &src_img, vector<Point2f> &src_vec)
{
    Size size(328, 778);
    Mat dst_img = Mat::zeros(size, CV_8UC3);
    vector<Point2f> dst_vec;

    dst_vec.push_back(Point2f(0, 0));
    dst_vec.push_back(Point2f(size.width - 1, 0));
    dst_vec.push_back(Point2f(size.width - 1, size.height - 1));
    dst_vec.push_back(Point2f(0, size.height - 1));
    namedWindow("Cropped Frame", 0);

    Mat H = findHomography(src_vec, dst_vec);
    warpPerspective(src_img, dst_img, H, size);

    imshow("Cropped Frame", dst_img);
    waitKey(0);

    unsigned int extension_position = img_name.find_last_of('.');
    string cropped_img_name = "";
    cropped_img_name += img_name;
    cropped_img_name.insert(extension_position, "_cropped");
    imwrite(cropped_img_name, dst_img);
}
