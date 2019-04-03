#ifndef BIGROCK_TRANSFORM_H
#define BIGROCK_TRANSFORM_H

#include "vector3.hpp"
#include "quaternion.hpp"
#include "transform.hpp"

namespace bigrock
{
    namespace math
    {
        class Transform
        {
            public:
            Vector3 origin;
            Quaternion rotation;
            Vector3 scale;

            // Constructors

            Transform();
            Transform(const Vector3 &origin, const Quaternion &rotation = Quaternion(), const Vector3 &scale = Vector3(1,1,1));

            // Methods

            // Move this transform in relative units
            void translate(const Vector3 &relative);
            void rotate(const Vector3 &axis, const float radians);
            void rotate_degrees(const Vector3 &axis, const float degrees);
            void rotate_euler(const Vector3 &degrees);

            // Take a global position and translate, rotate, and scale it to local coordinates
            Vector3 to_local(const Vector3 &global) const;
            // Take a local position and translate, rotate, and scale it to global coordinates
            Vector3 to_global(const Vector3 &local) const;

            // Produce a version of the Transform relative to this transform
            Transform xform(const Transform &t) const;
            // Take the relative Transform and produce a global form
            Transform xform_inv(const Transform &t) const;
        };
    }
}

#endif
