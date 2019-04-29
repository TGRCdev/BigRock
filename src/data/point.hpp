#ifndef BIGROCK_POINT_H
#define BIGROCK_POINT_H
#pragma once

#include <glm/vec3.hpp>

#include "../defines.hpp"

namespace bigrock {
namespace data {

struct Point
{
    Point();

    glm::vec3 position;
    float isovalue; // TODO: See if half float can be used here
    unsigned char material;

    ///Returns a point with interpolated isovalue and material.
    Point interpolate(const Point &other, const float &t) const;

    ///Returns true if the given points are similar enough to be truncated.
    bool can_collapse(const Point &other) const;
};

}}

#endif