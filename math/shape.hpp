#ifndef BIGROCK_SHAPE_H
#define BIGROCK_SHAPE_H

#include "vector3.hpp"
#include "transform.hpp"

namespace bigrock
{
    namespace math
    {
        class Shape
        {
            public:
            Transform transform;

            virtual bool contains_point(const Vector3 &point) const = 0;
            
            /**
             * Returns the point's isovalue relative to this shape.
             * If the value is >0, the point is inside of the shape
             * If the value is 1, the point is past the margin
             * If the value is 0, the point rests on the edge of the shape
             * If the value is <0, the point is outside of the shape
             * If the value is -1, the point is outside of the margin
             */
            virtual float get_isovalue(const Vector3 &point, const float margin=1.0) const = 0;
        };

        class Sphere : public Shape
        {
            public:
            float radius;

            bool contains_point(const Vector3 &point) const;
            float get_isovalue(const Vector3 &point, const float margin=1.0) const;
        };
    }
}

#endif