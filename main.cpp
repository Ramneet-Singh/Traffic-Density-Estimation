#include <iostream>
#include <chrono>
#include <angleCorrection.hpp>
#include <densityEstimation.hpp>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        if (argc < 2)
        {
            std::cout << "Missing Command Line Arguments\n";
            std::cout << "The correct usage is: " << argv[0] << " <video_file_path>\n";
        }
        else
        {
            std::cout << "Too Many Command Line Arguments\n";
            std::cout << "The correct usage is: " << argv[0] << " <video_file_path>\n";
        }
        return 1;
    }
    std::cout << "====================AVERAGE RUNNING TIME VS NUMBER OF THREADS================\n";
    for (int t = 1; t <= 6; t++)
    {
        float timeSum = 0.0;
        for (int n = 0; n < 10; n++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            outputSpatialParallelQueueDensity(argv[1], t);
            auto stop = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
            timeSum += duration.count();
        }
        float avgTime = timeSum / 10.0;
        std::cout << "              " << t << " THREADS  ----->   " << avgTime << " SECONDS\n";
    }
}
