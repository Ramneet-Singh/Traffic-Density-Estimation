#include <iostream>
#include <angleCorrection.hpp>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        if (argc < 2)
        {
            std::cout << "Missing Command Line Arguments\n";
            std::cout << "The correct usage is: " << argv[0] << " <image_file_path>\n";
        }
        else
        {
            std::cout << "Too Many Command Line Arguments\n";
            std::cout << "The correct usage is: " << argv[0] << " <image_file_path>\n";
        }
        return 1;
    }
    perspectiveCorrection(argv[1]);
    return 0;
}