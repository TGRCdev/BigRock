#ifndef BIGROCK_GRID_VERTICES_H
#define BIGROCK_GRID_VERTICES_H

#include "../../math/vector3.hpp"

namespace bigrock
{
    namespace terrain
    {
        const math::Vector3 GRID_VERTICES[8] = {
            math::Vector3(0,0,0),
            math::Vector3(1,0,0),
            math::Vector3(0,1,0),
            math::Vector3(1,1,0),
            math::Vector3(0,0,1),
            math::Vector3(1,0,1),
            math::Vector3(0,1,1),
            math::Vector3(1,1,1)
        };
    }
}

#endif