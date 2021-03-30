#include <densityEstimation.hpp>

using namespace cv;
using namespace std;

void outputSparseOpticalFlow(string videoPath)
{
    if (!utils::fs::exists("subtask3Outputs/"))
    {
        utils::fs::createDirectory("subtask3Outputs");
    }
    if (!utils::fs::exists("subtask3Outputs/method5/"))
    {
        utils::fs::createDirectory("subtask3Outputs/method5");
    }

    string fileName = "subtask3Outputs/method5/sparseOpticalFlowOut.txt";
    ofstream outFile(fileName);
    VideoCapture capture(videoPath);
    if (!capture.isOpened())
    {
        cerr << "Unable to open file!" << endl;
        exit(EXIT_FAILURE);
    }
    double timestamp;
    Mat old_frame, old_gray, old_frame1;
    vector<Point2f> p0, p1;
    Size size(328, 778);

    capture >> old_frame;
    Mat tform = computeHomography();
    warpPerspective(old_frame, old_frame1, tform, size);
    cvtColor(old_frame1, old_gray, COLOR_BGR2GRAY);

    goodFeaturesToTrack(old_gray, p0, 100, 0.3, 7, Mat(), 7, false, 0.04);
    int j = 0;
    while (true)
    {
        Mat frame1, frame_gray, frame;

        capture >> frame1;
        if (frame1.empty())
            break;
        timestamp = (capture.get(CAP_PROP_POS_MSEC)) / 1000.0;
        outFile << timestamp << ",";
        warpPerspective(frame1, frame, tform, size);
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);

        if (j == 5)
        {
            j = 0;
            goodFeaturesToTrack(old_gray, p0, 100, 0.3, 7, Mat(), 7, false, 0.04);
        }
        if (p0.empty())
        {
            goodFeaturesToTrack(old_gray, p0, 100, 0.3, 7, Mat(), 7, false, 0.04);
        }
        vector<uchar> status;
        vector<float> err;
        TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
        calcOpticalFlowPyrLK(old_gray, frame_gray, p0, p1, status, err, Size(15, 15), 2, criteria);

        vector<Point2f> good_new;
        float count = 0;
        for (uint i = 0; i < p0.size(); i++)
        {
            float dist = 0;
            if (i < p1.size())
            {
                float x0 = p0[i].x;
                float y0 = p0[i].y;
                float x1 = p1[i].x;
                float y1 = p1[i].y;
                dist = sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
            }
            if (dist > 2)
            {
                count++;
            }
            // Select good points
            if (status[i] == 1)
            {
                good_new.push_back(p1[i]);
            }
        }
        outFile << count / 125 << endl;

        old_gray = frame_gray.clone();
        p0 = good_new;
        j++;
    }
}