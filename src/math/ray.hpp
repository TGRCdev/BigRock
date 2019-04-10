#ifndef BIGROCK_RAY_H
#define BIGROCK_RAY_H

#include "vector3.hpp"

namespace bigrock
{
    namespace math
    {
        class Ray
        {
            Vector3 direction;

            public:
            Vector3 origin;

            Ray(const Vector3 &origin = Vector3::zero, const Vector3 &direction = Vector3::zero);

            Vector3 get_direction() const {return direction;}
            void set_direction(const Vector3 &direction);
            Vector3 get_point_on_ray(const float t) const;
            Vector3 get_closest_point(const Vector3 &target) const;
        };
    }
}

#endif
