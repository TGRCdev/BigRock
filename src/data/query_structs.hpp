#ifndef BIGROCK_QUERY_STRUCTS_H
#define BIGROCK_QUERY_STRUCTS_H
#pragma once

#include "../defines.hpp"

#include <map>
#include <glm/vec3.hpp>

namespace bigrock {
namespace data {

struct ToolQueryResult
{
    std::map<unsigned int, float> deltas; // TODO: REPLACE STD???
};

struct RayQueryResult
{
    bool hit;
    Vector3 pos;
    unsigned int material;
};

}}

#endif