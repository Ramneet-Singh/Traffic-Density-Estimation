#include <densityEstimation.hpp>

using namespace cv;
using namespace std;

void reducedResolutionQueueDensity(string videoPath, int X, int Y)
{
    VideoCapture capture(videoPath);
    if (!capture.isOpened())
    {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        exit(-1);
    }
    int X_ = COLNUM * X / 1920;
    int Y_ = ROWNUM * Y / 1080;
    Size size(X_, Y_);
    Mat tform = computeHomography(X, Y);
    Mat im_src2 = imread("resources/background.png");
    Mat im_src1(X_, Y_, CV_8UC1);
    resize(im_src2, im_src1, Size(X, Y));
    Mat im_src(X_, Y_, CV_8UC1);
    cv::cvtColor(im_src1, im_src, cv::COLOR_BGR2GRAY);
    Mat im_dst1 = Mat::zeros(size, CV_8UC1);
    warpPerspective(im_src, im_dst1, tform, size);
    Mat bgFrame1 = im_dst1;
    Mat bgFrame;
    resize(bgFrame1, bgFrame, size);
    Mat frame1, greyFrame, frame;
    Mat im = Mat::zeros(size, CV_8UC1);
    double timestamp;

    if (!utils::fs::exists("subtask3Outputs/"))
    {
        utils::fs::createDirectory("subtask3Outputs");
    }
    if (!utils::fs::exists("subtask3Outputs/method2/"))
    {
        utils::fs::createDirectory("subtask3Outputs/method2");
    }
    string filename = "subtask3Outputs/method2/reducedResQueueDensityOutput.txt";
    ofstream outfile(filename);
    while (true)
    {
        capture >> frame1;
        timestamp = (capture.get(CAP_PROP_POS_MSEC)) / 1000.0;
        if (frame1.empty())
        {
            break;
        }
        resize(frame1, frame, Size(X, Y));
        outfile << timestamp << ",";
        warpPerspective(frame, im, tform, size);
        cvtColor(im, greyFrame, COLOR_BGR2GRAY);
        Mat temp = greyFrame - bgFrame;
        float queueDen = 0;
        for (int y = 0; y < Y_; y++)
        {
            for (int x = 0; x < X_; x++)
            {
                if ((int)(temp.at<uint8_t>(y, x) >= 14))
                {
                    (temp.at<uint8_t>(y, x)) = 255;
                    queueDen++;
                }
                else
                {
                    (temp.at<uint8_t>(y, x)) = 0;
                }
            }
        }
        queueDen = queueDen / (X_ * (300 * Y / 1080));
        outfile << queueDen << "\n";
    }
    outfile.close();
}