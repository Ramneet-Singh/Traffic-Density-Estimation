#include <densityEstimation.hpp>

using namespace cv;
using namespace std;

void subSampledQueueDensity(string videoPath, int numSkip)
{
    VideoCapture capture(videoPath);
    if (!capture.isOpened())
    {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        exit(-1);
    }

    Mat tform = computeHomography();
    Mat bgFrame = getBackgroundFrame(tform);

    Mat frame, greyFrame;
    Size size(COLNUM, ROWNUM);
    Mat im = Mat::zeros(size, CV_8UC1);
    double timestamp;

    if (!utils::fs::exists("subtask3Outputs/"))
    {
        utils::fs::createDirectory("subtask3Outputs");
    }
    if (!utils::fs::exists("subtask3Outputs/method1/"))
    {
        utils::fs::createDirectory("subtask3Outputs/method1");
    }

    string filename = "subtask3Outputs/method1/subSampledQueueDensityOutput.txt";
    ofstream outfile(filename);
    while (true)
    {
        capture >> frame;
        timestamp = (capture.get(CAP_PROP_POS_MSEC)) / 1000.0;
        if (frame.empty())
        {
            break;
        }
        outfile << timestamp << ",";
        warpPerspective(frame, im, tform, size);
        cvtColor(im, greyFrame, COLOR_BGR2GRAY);
        float queueDen = computeQueueDensity(greyFrame, bgFrame);
        outfile << queueDen << "\n";
        for (int j = 1; j < numSkip; j++)
        {
            capture >> frame;
            timestamp = (capture.get(CAP_PROP_POS_MSEC)) / 1000.0;
            if (frame.empty())
            {
                break;
            }
            outfile << timestamp << "," << queueDen << "\n";
        }
    }
    outfile.close();
}