#ifndef BIGROCK_AABB_H
#define BIGROCK_AABB_H

#include "vector3.hpp"

namespace bigrock
{
    namespace math
    {
        struct AABB
        {
            AABB(Vector3 origin = Vector3::zero, Vector3 size = Vector3::zero);

            Vector3 origin;
            Vector3 size;
        };
    }
}

#endif