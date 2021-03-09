#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    ofstream outFile;
    outFile.open("opticalFlowOutput.csv");
    outFile << "Frame Number, "
            << "Dynamic Density\n";
    VideoCapture capture("resources/trafficvideo.mp4");
    if (!capture.isOpened())
    {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return 0;
    }
    Size size(328, 778);
    Mat im_1 = Mat::zeros(size, CV_8UC1);
    Mat im_2 = Mat::zeros(size, CV_8UC1);

    vector<Point2f> pts_dst;
    pts_dst.push_back(Point2f(0, 0));
    pts_dst.push_back(Point2f(size.width, 0));
    pts_dst.push_back(Point2f(size.width, size.height));
    pts_dst.push_back(Point2f(0, size.height));

    vector<Point2f> datap;
    datap.push_back(Point2f(1000, 220));
    datap.push_back(Point2f(1280, 220));
    datap.push_back(Point2f(1550, 1078));
    datap.push_back(Point2f(440, 1078));

    Mat frame1, prvs;
    capture >> frame1;

    Mat tform = findHomography(datap, pts_dst);
    warpPerspective(frame1, im_1, tform, size);

    cvtColor(im_1, prvs, COLOR_BGR2GRAY);

    long int frameNum = 0;
    while (true)
    {
        frameNum++;
        Mat frame2, next;
        capture >> frame2;
        if (frame2.empty())
            break;

        warpPerspective(frame2, im_2, tform, size);
        cvtColor(im_2, next, COLOR_BGR2GRAY);

        Mat flow(prvs.size(), CV_32FC2);
        calcOpticalFlowFarneback(prvs, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

        // visualization
        Mat flow_parts[2];
        split(flow, flow_parts);
        Mat magnitude, angle, magn_norm;
        cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
        normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
        angle *= ((1.f / 360.f) * (180.f / 255.f));
        float count = 0;
        for (int y = 0; y < 778; y++)
        {
            for (int x = 0; x < 328; x++)
            {
                if ((magn_norm.at<float>(y, x)) > 0.1)
                {
                    count++;
                }
            }
        }
        //build hsv image
        Mat _hsv[3], hsv, hsv8, bgr;
        _hsv[0] = angle;
        _hsv[1] = Mat::ones(angle.size(), CV_32F);
        _hsv[2] = magn_norm;
        merge(_hsv, 3, hsv);
        hsv.convertTo(hsv8, CV_8U, 255.0);
        cvtColor(hsv8, bgr, COLOR_HSV2BGR);
        count = count / (328 * 778);
        outFile << frameNum << ", " << count << "\n";
        imshow("frame2", bgr);

        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;

        prvs = next;
    }
    outFile.close();
}