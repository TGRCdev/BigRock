#ifndef BIGROCK_POINT_QUERY_H
#define BIGROCK_POINT_QUERY_H
#pragma once

#include "../defines.hpp"

namespace bigrock
{
    // Results from Cell::query_point()
    struct PointQuery
    {
        // The density of the Cell at the given position
        isovalue_t density;
        // The material of the Cell at the given position
        material_t material;
        // The depth of the leaf Cell containing the given position
        unsigned char depth;

        PointQuery(isovalue_t density = 1.0f, material_t material = 0, unsigned char depth = 0) : density(density), material(material), depth(depth) {};
    };
}

#endif