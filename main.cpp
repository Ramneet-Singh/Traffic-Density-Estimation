#include <iostream>
#include <chrono>
#include <angleCorrection.hpp>
#include <densityEstimation.hpp>

void timeMethod(std::string videoPath, int params[], int numParams, int methodNum)
{
    std::cout << "====================AVERAGE RUNNING TIME================\n";
    float timeSum = 0.0;
    for (int n = 0; n < 10; n++)
    {
        static std::chrono::_V2::system_clock::time_point start, stop;
        switch (methodNum)
        {
        case 1:
        {
            assert(numParams == 1);
            int numSkip = params[0];
            start = std::chrono::high_resolution_clock::now();
            subSampledQueueDensity(videoPath, numSkip);
            stop = std::chrono::high_resolution_clock::now();
            break;
        }
        case 2:
        {
            assert(numParams == 2);
            int dimX = params[0];
            int dimY = params[1];
            start = std::chrono::high_resolution_clock::now();
            reducedResolutionQueueDensity(videoPath, dimX, dimY);
            stop = std::chrono::high_resolution_clock::now();
            break;
        }
        case 3:
        {
            assert(numParams == 1);
            int numThreads = params[0];
            start = std::chrono::high_resolution_clock::now();
            outputSpatialParallelQueueDensity(videoPath, numThreads);
            stop = std::chrono::high_resolution_clock::now();
            break;
        }
        case 4:
        {
            assert(numParams == 1);
            int numThreads = params[0];
            start = std::chrono::high_resolution_clock::now();
            outputTemporalParallelQueueDensity(videoPath, numThreads);
            stop = std::chrono::high_resolution_clock::now();
            break;
        }
        case 5:
        {
            assert(numParams == 0);
            start = std::chrono::high_resolution_clock::now();
            outputSparseOpticalFlow(videoPath);
            stop = std::chrono::high_resolution_clock::now();
            break;
        }
        default:
        {
            exit(EXIT_FAILURE);
            break;
        }
        }

        auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
        timeSum += duration.count();
    }
    float avgTime = timeSum / 10.0;
    std::cout << "METHOD    " << methodNum << ":        PARAMETERS ";
    for (int m = 0; m < numParams; m++)
    {
        if (m == (numParams - 1))
        {
            std::cout << params[m];
        }
        else
        {
            std::cout << params[m] << ", ";
        }
    }
    std::cout << "  ----->   " << avgTime << "   SECONDS\n";
}
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Missing Command Line Arguments\n";
        std::cout << "The correct usage is: " << argv[0] << " <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>\n";
        exit(-1);
    }

    std::string videoPath = argv[1];
    int methodNum = std::stoi(argv[2]);
    if (argc < 4 && methodNum != 5)
    {
        std::cout << "Missing Command Line Arguments\n";
        std::cout << "The correct usage is: " << argv[0] << " <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>\n";
        exit(-1);
    }
    int param1, param2;
    int params[2];
    switch (methodNum)
    {
    case 1:
        param1 = std::stoi(argv[3]);
        if (argc == 5)
        {
            if (strcmp(argv[4], "-t"))
            {
                std::cout << "Incorrect Timing Flag\n";
                std::cout << "Use -t to set Timing Flag\n";
                std::cout << "The correct usage is: " << argv[0] << " <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>\n";
                exit(-1);
            }
            params[0] = param1;
            timeMethod(videoPath, params, 1, 1);
            exit(EXIT_SUCCESS);
        }
        else if (argc > 5)
        {
            std::cout << "Too Many Command Line Arguments\n";
            std::cout << "Method 1 (Sub Sampling) only takes 1 parameter : Number of Frames to Drop\n";
            std::cout << "The correct usage is: " << argv[0] << " <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>\n";
            exit(-1);
        }
        subSampledQueueDensity(videoPath, param1);
        break;
    case 2:
        param1 = std::stoi(argv[3]);
        if (argc < 5)
        {
            std::cout << "Missing Command Line Arguments\n";
            std::cout << "Method 2 (Reducing Frame Resolution) takes 2 parameters : Dimensions of Target Resolution\n";
            std::cout << "The correct usage is: " << argv[0] << " <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>\n";
            exit(-1);
        }
        param2 = std::stoi(argv[4]);
        if (argc == 6)
        {
            if (strcmp(argv[5], "-t"))
            {
                std::cout << "Incorrect Timing Flag\n";
                std::cout << "Use -t to set Timing Flag\n";
                std::cout << "The correct usage is: " << argv[0] << " <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>\n";
                exit(-1);
            }
            params[0] = param1;
            params[1] = param2;
            timeMethod(videoPath, params, 2, 2);
            exit(EXIT_SUCCESS);
        }
        else if (argc > 6)
        {
            std::cout << "Too Many Command Line Arguments\n";
            std::cout << "Method 2 (Reducing Frame Resolution) only takes 2 parameters : Dimensions of Target Resolution\n";
            std::cout << "The correct usage is: " << argv[0] << " <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>\n";
            exit(-1);
        }
        reducedResolutionQueueDensity(videoPath, param1, param2);
        break;
    case 3:
        param1 = std::stoi(argv[3]);
        if (argc == 5)
        {
            if (strcmp(argv[4], "-t"))
            {
                std::cout << "Incorrect Timing Flag\n";
                std::cout << "Use -t to set Timing Flag\n";
                std::cout << "The correct usage is: " << argv[0] << " <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>\n";
                exit(-1);
            }
            params[0] = param1;
            timeMethod(videoPath, params, 1, 3);
            exit(EXIT_SUCCESS);
        }
        else if (argc > 5)
        {
            std::cout << "Too Many Command Line Arguments\n";
            std::cout << "Method 3 (Spatial Multithreading) only takes 1 parameter : Number of Threads\n";
            std::cout << "The correct usage is: " << argv[0] << " <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>\n";
            exit(-1);
        }
        else
        {
            outputSpatialParallelQueueDensity(videoPath, param1);
        }
        break;
    case 4:
        param1 = std::stoi(argv[3]);
        if (argc == 5)
        {
            if (strcmp(argv[4], "-t"))
            {
                std::cout << "Incorrect Timing Flag\n";
                std::cout << "Use -t to set Timing Flag\n";
                std::cout << "The correct usage is: " << argv[0] << " <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>\n";
                exit(-1);
            }
            params[0] = param1;
            timeMethod(videoPath, params, 1, 4);
            exit(EXIT_SUCCESS);
        }
        else if (argc > 5)
        {
            std::cout << "Too Many Command Line Arguments\n";
            std::cout << "Method 4 (Temporal Multithreading) only takes 1 parameter : Number of Threads\n";
            std::cout << "The correct usage is: " << argv[0] << " <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>\n";
            exit(-1);
        }
        outputTemporalParallelQueueDensity(videoPath, param1);
        break;
    case 5:
        if (argc == 4)
        {
            if (strcmp(argv[3], "-t"))
            {
                std::cout << "Incorrect Timing Flag\n";
                std::cout << "Use -t to set Timing Flag\n";
                std::cout << "The correct usage is: " << argv[0] << " <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>\n";
                exit(-1);
            }
            timeMethod(videoPath, params, 0, 5);
            exit(EXIT_SUCCESS);
        }
        else if (argc > 5)
        {
            std::cout << "Too Many Command Line Arguments\n";
            std::cout << "Method 5 (Sparse Optical Flow) takes 0 Parameters\n";
            std::cout << "The correct usage is: " << argv[0] << " <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>\n";
            exit(-1);
        }
        outputSparseOpticalFlow(videoPath);
        break;
    default:
        std::cout << "Missing Command Line Arguments\n";
        std::cout << "Method Number must be between 1 and 4\n";
        std::cout << "The correct usage is: " << argv[0] << " <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>\n";
        exit(-1);
    }
}