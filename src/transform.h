#ifndef BIGROCK_TRANSFORM_H
#define BIGROCK_TRANSFORM_H

#include "vector3.h"
#include "quaternion.h"

namespace bigrock
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
        void translate(const Vector3 &relative); // Move this transform in relative units
        void rotate(const Vector3 &axis, const float radians);
        void rotate_degrees(const Vector3 &axis, const float degrees);
        void rotate_euler(const Vector3 &degrees);

        Vector3 to_local(const Vector3 &global) const; // Take a global position and translate, rotate, and scale it to local coordinates
        Vector3 to_global(const Vector3 &local) const; // Take a local position and translate, rotate, and scale it to global coordinates
    };
}

#endif
