#include <iostream>
#include <angleCorrection.hpp>

using namespace std;
using namespace cv;

void mouseHandler(int event, int x, int y, int flags, void *data_ptr)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        userdata *data = ((userdata *)data_ptr);
        circle(data->im, Point(x, y), 3, Scalar(0, 255, 255), 5, cv::LINE_AA);
        imshow("Original Frame", data->im);
        if (data->points.size() < 4)
        {
            data->points.push_back(Point2f(x, y));
        }
    }
}

void perspectiveCorrection(string img_name)
{
    Mat img1;
    Mat img = imread(img_name);

    if (img.empty())
    {
        cout << "\nCould not read image: " << img_name << "\n";
        cout << "Please check the path to your image\n";
        return;
    }

    cv::cvtColor(img, img1, cv::COLOR_BGR2GRAY);
    Mat img2 = Mat::zeros(img1.size(), CV_8UC3);
    Size size(1500, 850);
    vector<Point2f> pts_dst;

    pts_dst.push_back(Point2f(472, 52));
    pts_dst.push_back(Point2f(800, 52));
    pts_dst.push_back(Point2f(800, 830));
    pts_dst.push_back(Point2f(472, 830));

    //Create a window
    namedWindow("Original Frame", 0);

    Mat im_temp = img1.clone();

    userdata data;
    data.im = im_temp;

    //set the callback function for any mouse event
    setMouseCallback("Original Frame", mouseHandler, &data);

    //show the Original Frame
    imshow("Original Frame", im_temp);
    waitKey(0);

    //! [estimate-homography]
    Mat H = findHomography(data.points, pts_dst);
    //! [estimate-homography]

    //! [warp]
    Mat img1_warp;
    warpPerspective(img1, img1_warp, H, size);
    //! [warp]

    namedWindow("Projected Frame", 0);
    imshow("Projected Frame", img1_warp);
    waitKey(0);
    unsigned int extension_position = img_name.find_last_of('.');
    string projected_img_name = "";
    projected_img_name += img_name;
    projected_img_name.insert(extension_position, "_projected");
    imwrite(projected_img_name, img1_warp);
    transform(img_name, img, data.points);
}