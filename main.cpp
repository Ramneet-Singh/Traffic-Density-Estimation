#include <iostream>
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
    std::pair<std::vector<float>, std::vector<float>> p1 = queueDensity(argv[1]);
    std::vector<float> times = p1.first;
    std::vector<float> queueDens = p1.second;
    std::vector<float> dynamicDens = dynamicDenVals(argv[1]);

    for (int index = 0; index < dynamicDens.size(); index++)
    {
        std::cout << times[index] << "," << queueDens[index] << "," << dynamicDens[index] << "\n";
    }
    return 0;
}