#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <chrono>

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

int main(int argc, char **argv)
{
    static std::chrono::_V2::system_clock::time_point start, stop;
    start = std::chrono::high_resolution_clock::now();
    ofstream outfile("SparseOptFlowOut.txt");
    VideoCapture capture("trafficvideo.mp4");
    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return 0;
    }
    double timestamp;
    Mat old_frame, old_gray, old_frame1;
    vector<Point2f> p0, p1;
    Size size(328, 778);
    // Take first frame and find corners in it
    capture >> old_frame;
    Mat tform = computeHomography();
    warpPerspective(old_frame, old_frame1, tform, size);
    cvtColor(old_frame1, old_gray, COLOR_BGR2GRAY);
    
    goodFeaturesToTrack(old_gray, p0, 100, 0.3, 7, Mat(), 7, false, 0.04);

    // Create a mask image for drawing purposes
    Mat mask = Mat::zeros(old_frame1.size(), old_frame1.type());
    int j = 0;
    while(true){
        Mat frame1, frame_gray, frame;

        capture >> frame1;
        if (frame1.empty())
            break;
        timestamp = (capture.get(CAP_PROP_POS_MSEC)) / 1000.0;
        outfile << timestamp << ",";
        warpPerspective(frame1, frame, tform, size);
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        
        if (j == 5){
        	j=0;
        	goodFeaturesToTrack(old_gray, p0, 100, 0.3, 7, Mat(), 7, false, 0.04);
        }
        if (p0.empty()){
            goodFeaturesToTrack(old_gray, p0, 100, 0.3, 7, Mat(), 7, false, 0.04);
        }
        // calculate optical flow
        vector<uchar> status;
        vector<float> err;
        TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
        calcOpticalFlowPyrLK(old_gray, frame_gray, p0, p1, status, err, Size(15,15), 2, criteria);

        vector<Point2f> good_new;
        float count = 0;
        for(uint i = 0; i < p0.size(); i++)
        {
            float dist = 0;
            if (i < p1.size()){
                float x0 = p0[i].x;
                float y0 = p0[i].y;
                float x1 = p1[i].x;
                float y1 = p1[i].y;
                dist = sqrt(pow(x1-x0,2) + pow(y1-y0,2));
            }
            if (dist > 2) {count++;}
            // Select good points
            if(status[i] == 1) {
                good_new.push_back(p1[i]);
                // draw the tracks
            }
        }
        outfile<<count/125<<endl;
        //Mat img;
        //add(frame, mask, img);

        //imshow("Frame", img);

        //int keyboard = waitKey(30);
        //if (keyboard == 'q' || keyboard == 27)
         //   break;

        // Now update the previous frame and previous points
        old_gray = frame_gray.clone();
        p0 = good_new;
        j++;
    }
    stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    cout<< duration.count()<< endl;
}
