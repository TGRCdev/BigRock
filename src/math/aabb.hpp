#ifndef BIGROCK_AABB_H
#define BIGROCK_AABB_H

#include "vector3.hpp"

namespace bigrock
{
    namespace math
    {
        // Based on the AABB class from the Godot Engine
        struct AABB
        {
            AABB(const Vector3 &origin = Vector3::zero, const Vector3 &size = Vector3::zero);

            Vector3 position;
            Vector3 size;

            // Returns true if at least one axis's size is <= 0
            bool has_no_area() const;
            // Returns true if all axes of size is <= 0
            bool has_no_surface() const;

            // Returns true if both AABBs overlap
            bool intersects(const AABB &other) const;
            // Returns true if this AABB completely contains the other
            bool encloses(const AABB &other) const;
            // Returns true if the point falls within this AABB
            bool contains_point(const Vector3 &point) const;

            // Returns the intersection area between both AABBs
            AABB intersection(const AABB &other) const;
        };
    }
}

#endif