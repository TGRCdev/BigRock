#ifndef BIGROCK_VERTEX_H
#define BIGROCK_VERTEX_H

#include "../../math/vector3.hpp"

namespace bigrock {
namespace terrain {
namespace mesh {

struct Vertex
{
    Vertex(const math::Vector3 &position = math::Vector3::zero, const math::Vector3 &normal = math::Vector3::zero, const float &tx = 0.0, const float &ty = 0.0);

    math::Vector3 position; // Position
    math::Vector3 normal; // Normal
    float tx, ty; // Tex coords

    bool operator==(const Vertex &other) const;
    bool operator!=(const Vertex &other) const;
};

}}}

#endif