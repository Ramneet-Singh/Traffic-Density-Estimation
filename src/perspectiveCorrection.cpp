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
    Mat image1;
    Mat img = imread(img_name);

    if (img.empty())
    {
        cout << "\nCould not read image: " << img_name << "\n";
        cout << "Please check the path to your image\n";
        return;
    }

    cv::cvtColor(img, image1, cv::COLOR_BGR2GRAY);
    Mat img2 = Mat::zeros(image1.size(), CV_8UC3);
    Size size(1500, 850);
    vector<Point2f> dst_vec;

    dst_vec.push_back(Point2f(472, 52));
    dst_vec.push_back(Point2f(800, 52));
    dst_vec.push_back(Point2f(800, 830));
    dst_vec.push_back(Point2f(472, 830));
    
    namedWindow("Original Frame", 0);
    Mat temp = image1.clone();

    userdata data;
    data.im = temp;

    setMouseCallback("Original Frame", mouseHandler, &data);
    
    imshow("Original Frame", temp);
    waitKey(0);
    
    Mat H = findHomography(data.points, dst_vec);
    Mat image1_warp;
    warpPerspective(image1, image1_warp, H, size);
    
    namedWindow("Projected Frame", 0);
    imshow("Projected Frame", image1_warp);
    waitKey(0);
    unsigned int extension_position = img_name.find_last_of('.');
    string projected_img_name = "";
    projected_img_name += img_name;
    projected_img_name.insert(extension_position, "_projected");
    imwrite(projected_img_name, image1_warp);
    transform(img_name, img, data.points);
}
