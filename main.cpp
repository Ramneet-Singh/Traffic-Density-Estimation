#include <iostream>
#include <chrono>
#include <angleCorrection.hpp>
#include <densityEstimation.hpp>

void timeParallelQueueDensity(std::string videoPath, int numThreads, bool spatial)
{
    std::cout << "====================AVERAGE RUNNING TIME================\n";
    float timeSum = 0.0;
    for (int n = 0; n < 10; n++)
    {
        static std::chrono::_V2::system_clock::time_point start, stop;
        if (spatial)
        {
            start = std::chrono::high_resolution_clock::now();
            outputSpatialParallelQueueDensity(videoPath, numThreads);
            stop = std::chrono::high_resolution_clock::now();
        }
        else
        {
            start = std::chrono::high_resolution_clock::now();
            outputTemporalParallelQueueDensity(videoPath, numThreads);
            stop = std::chrono::high_resolution_clock::now();
        }

        auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
        timeSum += duration.count();
    }
    float avgTime = timeSum / 10.0;
    std::cout << "              " << numThreads << " THREADS  ----->   " << avgTime << " SECONDS\n";
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cout << "Missing Command Line Arguments\n";
        std::cout << "The correct usage is: " << argv[0] << " <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>\n";
        exit(-1);
    }

    std::string videoPath = argv[1];
    int methodNum = std::stoi(argv[2]);
    int param1, param2;
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
            else if (argc > 5)
            {
                std::cout << "Too Many Command Line Arguments\n";
                std::cout << "Method 2 (Reducing Frame Resolution) only takes 2 parameters : Dimensions of Target Resolution\n";
                std::cout << "The correct usage is: " << argv[0] << " <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>\n";
                exit(-1);
            }
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
            timeParallelQueueDensity(videoPath, param1, true);
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
            timeParallelQueueDensity(videoPath, param1, false);
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
    default:
        std::cout << "Missing Command Line Arguments\n";
        std::cout << "Method Number must be between 1 and 4\n";
        std::cout << "The correct usage is: " << argv[0] << " <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>\n";
        exit(-1);
    }
}