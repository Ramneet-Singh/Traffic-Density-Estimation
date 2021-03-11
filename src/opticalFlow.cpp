#include <densityEstimation.hpp>

using namespace cv;
using namespace std;

Mat computeHomography()
{
    Size size(328, 778);
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

    return (findHomography(datap, pts_dst));
}

void computeDynamicDensity(Mat &prev, Mat &nxt, float &dynDen /*promise<float> && prom*/)
{
    Mat flow(prev.size(), CV_32FC2);
    calcOpticalFlowFarneback(prev, nxt, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

    // visualization
    Mat flow_parts[2];
    split(flow, flow_parts);
    Mat magnitude, angle, magn_norm;
    cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
    normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
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

    count = count / (328 * 778);

    dynDen = count;
    //prom.set_value(count);
    return;
}

void outputQueueAndDynamic(string videoPath)
{
    VideoCapture capture(videoPath);
    if (!capture.isOpened())
    {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return;
    }

    Size size(328, 778);
    Mat im_1 = Mat::zeros(size, CV_8UC1);
    Mat im_2 = Mat::zeros(size, CV_8UC1);

    Mat frame1, prvs;
    capture >> frame1;

    Mat tform = computeHomography();
    warpPerspective(frame1, im_1, tform, size);

    cvtColor(im_1, prvs, COLOR_BGR2GRAY);

    Mat bgFrame = getBackgroundFrame(tform);

    double timestamp;

    while (true)
    {
        Mat frame2, next;
        timestamp = (capture.get(CAP_PROP_POS_MSEC)) / 1000.0;
        capture >> frame2;
        if (frame2.empty())
            break;

        cout << timestamp << ",";

        warpPerspective(frame2, im_2, tform, size);
        cvtColor(im_2, next, COLOR_BGR2GRAY);
        /*promise<float> p;
        auto f = p.get_future();*/
        float dynamDen;
        thread dynamicThread(&computeDynamicDensity, ref(prvs), ref(next), ref(dynamDen));

        float queueDen = computeQueueDensity(next, bgFrame);
        cout << queueDen << ",";

        // Wait for dynamic density to be computed
        dynamicThread.join();
        cout << dynamDen << "\n";

        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;

        prvs = next;
    }
    return;
}