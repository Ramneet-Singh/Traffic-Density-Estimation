#include <iostream>
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
    //outputSpatialParallelQueueDensity(argv[1], std::stoi(argv[2]));
    combineParallelOutFiles(std::stoi(argv[2]), "threadOutputs", true);
}