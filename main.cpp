#include <iostream>
#include <chrono>
#include <angleCorrection.hpp>
#include <densityEstimation.hpp>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        if (argc < 3)
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
    auto start = std::chrono::high_resolution_clock::now();
    outputSpatialParallelQueueDensity(argv[1], std::stoi(argv[2]));
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Running Time of the Spatial Parallel Method for Queue Density is " << duration.count() << " seconds" << std::endl;
}
