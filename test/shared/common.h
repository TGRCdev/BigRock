#ifndef TEST_COMMON_H
#define TEST_COMMON_H
#pragma once

#include <algorithm>
#include <glm/vec3.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

void print_vec3(glm::vec3 vec)
{
    std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}

float rand_range(float min, float max)
{
    float t = float(rand()) / RAND_MAX;
    float diff = max - min;
    return min + (diff * t);
}

std::string load_binary_file_as_string(const std::string &file_name)
{
    std::fstream in(file_name, std::ios::binary | std::ios::in);
    if(!in.is_open())
        return std::string();
    
    std::stringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

#endif