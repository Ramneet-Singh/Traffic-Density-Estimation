#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Missing Command Line Arguments\n";
        std::cout << "The correct usage is: " << argv[0] << " <file1_path> <file2_path>\n";
        exit (-1);
    }
    else if (argc > 3)
    {
        std::cout << "Too Many Command Line Arguments\n";
        std::cout << "The correct usage is: " << argv[0] << " <file1_path> <file2_path>\n";
        exit (-1);
    }
    std::string path1 = argv[1];
    std::string path2 = argv[2];
    std::ifstream file1;
    file1.open(path1);
    if (!file1.is_open()){
        std::cerr<<"Can't read file1\n";
        exit(-1);
    }
    std::vector<float> vec;
    std::string line;
    while(getline(file1, line))
    {
        std::stringstream ss(line);
        std::string str1;
        getline(ss, str1, ',');
        getline(ss, str1, ',');
        if (str1.empty()){
            std::cerr<<"Error\n";
            exit(-1);
        }
        vec.push_back(std::stof(str1));
    }
    std::ifstream file2;
    file2.open(path2);
    if (!file2.is_open()){
        std::cerr<<"Can't read file2\n";
        exit(-1);
    }
    std::vector<float> vec2;
    std::string line2;
    while(getline(file2, line2))
    {
        std::stringstream ss2(line2);
        std::string str2;
        getline(ss2, str2, ',');
        getline(ss2, str2, ',');
        if (str2.empty()){
            std::cerr<<"Error\n";
            exit(-1);
        }
        vec2.push_back(std::stof(str2));
    }
    if (vec2.size() != vec.size()){
        std::cerr<<"Inappropriate Files\n";
        exit(-1);
    }
    float val = 0;
    for (int i=0; i<vec.size(); i++)
    {
        float num1 = vec.at(i);
        float num2 = vec2.at(i);
        val += (num1-num2)*(num1-num2);
    }
    file1.close();
    file2.close();
    std::cout<<"Error: (1/utility): "<<val<<"\n";
}