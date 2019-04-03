#include "aabb.hpp"

namespace bigrock
{
    namespace math
    {
        AABB::AABB(Vector3 origin, Vector3 size)
        {
            this->origin = origin;
            this->size = size;
        }
    }
}