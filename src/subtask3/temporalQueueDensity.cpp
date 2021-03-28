#include "densityEstimation.hpp"

using namespace std;
using namespace cv;

void *temporalParallelThreadFunc(void *arg)
{
    struct temporalParallelThreadArgs *myArg;
    myArg = (struct temporalParallelThreadArgs *)arg;
    int threadNum = myArg->threadId;
    int numSplits = myArg->numThreads;
    string videoPath = myArg->videoPath;

    Mat tform = computeHomography();
    Mat bgFrame = getBackgroundFrame(tform);

    Mat frame, greyFrame;
    Size size(COLNUM, ROWNUM);
    Mat im = Mat::zeros(size, CV_8UC1);
    double timestamp;

    VideoCapture capture(videoPath);
    if (!capture.isOpened())
    {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        exit(-1);
    }

    int totalFrames = capture.get(CAP_PROP_FRAME_COUNT);
    int frameWidth = totalFrames / numSplits;
    int startFrame = threadNum * frameWidth;
    int endFrame;
    if (threadNum == (numSplits - 1))
    {
        endFrame = totalFrames - 1;
    }
    else
    {
        endFrame = startFrame + frameWidth - 1;
    }
    cout << "THREAD: " << threadNum << " : " << startFrame << " TO " << endFrame << endl;
    capture.set(CAP_PROP_POS_FRAMES, startFrame);
    /*
    if (capture.get(CAP_PROP_POS_FRAMES) != startFrame)
    {
        capture.set(CAP_PROP_POS_FRAMES, startFrame + 1);
        if (capture.get(CAP_PROP_POS_FRAMES) != startFrame)
        {
            capture.set(CAP_PROP_POS_FRAMES, startFrame - 1);
            if (capture.get(CAP_PROP_POS_FRAMES) != startFrame)
            {
                cerr << "I GIVE UP THIS IS TOO HARD BYE" << endl;
                exit(-1);
            }
        }
    }
    */
    if (numSplits == 4 && threadNum == 2)
    {
        capture.read(frame);
    }

    if (!utils::fs::exists("subtask3Outputs/"))
    {
        utils::fs::createDirectory("subtask3Outputs");
    }
    if (!utils::fs::exists("subtask3Outputs/method4/"))
    {
        utils::fs::createDirectory("subtask3Outputs/method4");
    }

    string fileName = "subtask3Outputs/method4/temporalParallelQueueOut--" + to_string(threadNum) + ".txt";
    ofstream outFile(fileName);
    int j = 0;
    while (j < (endFrame - startFrame + 1))
    {
        capture.read(frame);
        timestamp = capture.get(CAP_PROP_POS_MSEC) / 1000.0;

        outFile << timestamp << ",";
        warpPerspective(frame, im, tform, size);
        cvtColor(im, greyFrame, COLOR_BGR2GRAY);

        float queueDen = computeQueueDensity(greyFrame, bgFrame);
        outFile << queueDen << "\n";
        j++;
    }
    cout << "THREAD: " << threadNum << " : TILL " << (j + startFrame - 1) << endl;
    outFile.close();
    pthread_exit(NULL);
}

void outputTemporalParallelQueueDensity(string videoPath, int numSplits)
{
    /*
        Divides the video into N time chunks, and gives each chunk to a separate thread to process
    */
    int i = 0;
    int rc;
    vector<pthread_t> threadIds;
    threadIds.reserve(numSplits);

    pthread_attr_t attr;
    // Initialize and set thread joinable
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    vector<struct temporalParallelThreadArgs> threadArgsVector;
    threadArgsVector.reserve(numSplits);
    while (i < numSplits)
    {
        int k = i++;
        int k1 = numSplits;
        pthread_t tid;
        struct temporalParallelThreadArgs args;
        args.threadId = k;
        args.numThreads = k1;
        args.videoPath = videoPath;
        threadArgsVector.push_back(args);

        rc = pthread_create(&tid, &attr, temporalParallelThreadFunc, (void *)(&threadArgsVector[k]));
        if (rc)
        {
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
        threadIds.push_back(tid);
    }

    for (int j = 0; j < numSplits; j++)
    {
        pthread_join(threadIds[j], NULL);
    }
    return;
}