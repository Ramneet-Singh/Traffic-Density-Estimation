#include <angleCorrection.hpp>

using namespace cv;
using namespace std;

void transform(string &img_name, Mat &im_src, vector<Point2f> &pts_src)
{
    // Destination image
    Size size(328, 778);
    Mat im_dst = Mat::zeros(size, CV_8UC3);

    // Create a vector of points.
    vector<Point2f> pts_dst;

    pts_dst.push_back(Point2f(0, 0));
    pts_dst.push_back(Point2f(size.width - 1, 0));
    pts_dst.push_back(Point2f(size.width - 1, size.height - 1));
    pts_dst.push_back(Point2f(0, size.height - 1));

    //Create a window
    namedWindow("Cropped Frame", 0);

    Mat tform = findHomography(pts_src, pts_dst);
    warpPerspective(im_src, im_dst, tform, size);

    imshow("Cropped Frame", im_dst);
    waitKey(0);

    unsigned int extension_position = img_name.find_last_of('.');
    string cropped_img_name = "";
    cropped_img_name += img_name;
    cropped_img_name.insert(extension_position, "_cropped");
    imwrite(cropped_img_name, im_dst);
}
