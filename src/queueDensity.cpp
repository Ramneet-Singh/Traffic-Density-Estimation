#include <densityEstimation.hpp>

#define ROWNUM 778
#define COLNUM 328

using namespace cv;
using namespace std;

Mat getBackgroundFrame(Mat transformMat)
{
	Mat im_src1 = imread("resources/background.png");
	Mat im_src(COLNUM, ROWNUM, CV_8UC1);
	cv::cvtColor(im_src1, im_src, cv::COLOR_BGR2GRAY);
	Size size(COLNUM, ROWNUM);
	Mat im_dst1 = Mat::zeros(size, CV_8UC1);
	warpPerspective(im_src, im_dst1, transformMat, size);

	return im_dst1;
}

float computeQueueDensity(Mat &currentFrame, Mat &backgroundFrame)
{
	Mat temp = currentFrame - backgroundFrame;
	float count = 0;
	for (int y = 0; y < ROWNUM; y++)
	{
		for (int x = 0; x < COLNUM; x++)
		{
			if ((int)(temp.at<uint8_t>(y, x) >= 14))
			{
				(temp.at<uint8_t>(y, x)) = 255;
				count++;
			}
			else
			{
				(temp.at<uint8_t>(y, x)) = 0;
			}
		}
	}
	count = count / (COLNUM * 300);

	return count;
}

void outputSpatialParallelQueueDensity(string videoPath, int numSplits)
{
	/*
		This will split work spatially among different threads, i.e., split each frame into parts and give each part to a different thread to process. Each thread will subtract the corresponding part of the background frame from the given frame and return its density. We then combine all of these densities by taking the mean and output our density
	*/
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

	pthread_attr_t attr;
	// Initialize and set thread joinable
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	int rc;

	/*
	string fileName = "threadOutputs/spatialParallelQueueOut--FINAL.txt";
	ofstream outFile(fileName);
	*/
	while (true)
	{
		int next_id = 0;
		timestamp = (capture.get(CAP_PROP_POS_MSEC)) / 1000.0;
		capture >> frame;
		if (frame.empty())
		{
			break;
		}

		// outFile << timestamp << ",";

		warpPerspective(frame, im, tform, size);
		cvtColor(im, greyFrame, COLOR_BGR2GRAY);

		// Declare N threads
		vector<pthread_t> threadIds;
		threadIds.reserve(numSplits);

		// Create Args for N threads
		vector<struct spatialParallelThreadArgs> threadArgsVector;
		threadArgsVector.reserve(numSplits);
		int startX = 0;
		int stripWidth = COLNUM / numSplits;
		while (next_id < (numSplits - 1))
		{
			int i = next_id++;
			pthread_t tid;
			struct spatialParallelThreadArgs sparg;
			sparg.threadId = i;
			sparg.bgFrameBlk = getFrameBlk(bgFrame, startX, stripWidth);
			sparg.curFrameBlk = getFrameBlk(greyFrame, startX, stripWidth);
			threadArgsVector.push_back(sparg);

			rc = pthread_create(&tid, &attr, spatialParallelThreadFunc, (void *)(&threadArgsVector[i]));
			if (rc)
			{
				cout << "Error:unable to create thread," << rc << endl;
				exit(-1);
			}
			threadIds.push_back(tid);
			startX += stripWidth;
		}
		int k = next_id++;
		pthread_t tid;
		struct spatialParallelThreadArgs sparg;
		sparg.threadId = k;
		sparg.bgFrameBlk = getFrameBlk(bgFrame, startX, COLNUM - startX);
		sparg.curFrameBlk = getFrameBlk(greyFrame, startX, COLNUM - startX);
		threadArgsVector.push_back(sparg);

		rc = pthread_create(&tid, &attr, spatialParallelThreadFunc, (void *)(&threadArgsVector[k]));
		if (rc)
		{
			cout << "Error:unable to create thread," << rc << endl;
			exit(-1);
		}
		threadIds.push_back(tid);

		vector<float> threadResults;
		float sum = 0.0;
		for (int m = 0; m < numSplits; m++)
		{
			float *res;
			pthread_join(threadIds[m], (void **)&res);
			sum += *res;
		}
		float queueDen = sum / (COLNUM * 300.0);
		// outFile << queueDen << "\n";
		threadIds.clear();
		threadArgsVector.clear();
		threadResults.clear();
	}

	// outFile.close();
	return;
}

Mat getFrameBlk(Mat &frame, int startCoordinate, int blockSize)
{
	return frame(Rect(startCoordinate, 0, blockSize, frame.rows)).clone();
}

void *spatialParallelThreadFunc(void *arg)
{
	struct spatialParallelThreadArgs *myArgs;
	myArgs = (struct spatialParallelThreadArgs *)arg;
	Mat frameBlk = myArgs->curFrameBlk;
	Mat bgBlk = myArgs->bgFrameBlk;
	Mat temp = frameBlk - bgBlk;
	float count = 0;
	for (int y = 0; y < temp.rows; y++)
	{
		for (int x = 0; x < temp.cols; x++)
		{
			if ((int)(temp.at<uint8_t>(y, x) >= 14))
			{
				(temp.at<uint8_t>(y, x)) = 255;
				count++;
			}
			else
			{
				(temp.at<uint8_t>(y, x)) = 0;
			}
		}
	}
	pthread_exit((void *)&count);
}

void combineParallelOutFiles(int numThreads, string outputDir, bool spatial)
{
	int k = 0;
	vector<unique_ptr<ifstream>> outFiles;
	string prefix = spatial ? "spatial" : "temporal";
	while (k < numThreads)
	{
		string fileName = outputDir + "/" + prefix + "ParallelQueueOut--" + to_string(k);
		unique_ptr<ifstream> file(new ifstream(fileName));
		outFiles.push_back(move(file));
		k++;
	}
	ofstream outputFile(outputDir + "/" + prefix + "ParallelQueueOut--FINAL");
	string line;
	while (getline(*outFiles[0].get(), line))
	{
		vector<string> outLines;
		outLines.reserve(numThreads);
		outLines.push_back(line);
		for (int j = 1; j < numThreads; j++)
		{
			getline(*outFiles[j], line);
			outLines.push_back(line);
		}
		int start = 0;
		int end = outLines[0].find(",");
		outputFile << outLines[0].substr(start, end - start) << ",";
		float denSum = 0.0;
		for (int i = 0; i < numThreads; i++)
		{
			start = 0;
			end = outLines[i].find(",");
			float countVal = stof(outLines[i].substr(end + 1, outLines[i].size() - end - 1));
			denSum += countVal;
		}

		float queueDen = denSum / (COLNUM * 300);
		outputFile << queueDen << "\n";
	}
}