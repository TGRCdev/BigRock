#ifndef TEST_COMMON_H
#define TEST_COMMON_H
#pragma once

#include <algorithm>
#include <glm/vec3.hpp>
#include <iostream>

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

#endif