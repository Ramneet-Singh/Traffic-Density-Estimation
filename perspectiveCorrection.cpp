#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

struct userdata{
    Mat im;
    vector<Point2f> points;
};


void mouseHandler(int event, int x, int y, int flags, void* data_ptr)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {
        userdata *data = ((userdata *) data_ptr);
        circle(data->im, Point(x,y),3,Scalar(0,255,255), 5, cv::LINE_AA);
        imshow("Image", data->im);
        if (data->points.size() < 4)
        {
            data->points.push_back(Point2f(x,y));
        }
    }
}

void perspectiveCorrection(string img_name)
{
    Mat img1;
    Mat img = imread(img_name);
    cv::cvtColor(img, img1, cv::COLOR_BGR2GRAY);
    Mat img2 = Mat::zeros(img1.size(),CV_8UC3);
    Size size(1500, 850);
    vector<Point2f> pts_dst;

    pts_dst.push_back(Point2f(472,52));
    pts_dst.push_back(Point2f(800,52));
    pts_dst.push_back(Point2f(800,830));
    pts_dst.push_back(Point2f(472,830));
	
    //Create a window
    namedWindow("Image", 0);

    Mat im_temp = img1.clone();

    userdata data;
    data.im = im_temp;

    //set the callback function for any mouse event
    setMouseCallback("Image", mouseHandler, &data);
    
    //show the image
    imshow("Image", im_temp);
    waitKey(0);
    
    //! [estimate-homography]
    Mat H = findHomography(data.points, pts_dst);
    //! [estimate-homography]

    //! [warp]
    Mat img1_warp;
    warpPerspective(img1, img1_warp, H, size);
    //! [warp]
    
    namedWindow("Output", 0);
    imshow("Output", img1_warp);

    waitKey();
}

int main(int argc, char *argv[])
{
    perspectiveCorrection("empty.jpg");

    return 0;
}
