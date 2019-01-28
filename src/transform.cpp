#include "transform.h"
#include "vector3.h"
#include "quaternion.h"

namespace bigrock
{
    #pragma region Constructors
    Transform::Transform()
    {
        this->origin = Vector3();
        this->rotation = Quaternion();
        this->scale = Vector3(1,1,1);
    }

    Transform::Transform(const Vector3 &origin, const Quaternion &rotation, const Vector3 &scale)
    {
        this->origin = origin;
        this->rotation = rotation;
        this->scale = scale;
    }
    #pragma endregion

    #pragma region Methods
    void Transform::translate(const Vector3 &relative)
    {
        origin += to_global(relative);
    }

    void Transform::rotate(const Vector3 &axis, const float radians)
    {
        rotation.rotate_axis(axis, radians);
    }

    void Transform::rotate_degrees(const Vector3 &axis, const float degrees)
    {
        rotation.rotate_axis_degrees(axis, degrees);
    }

    void Transform::rotate_euler(const Vector3 &radians)
    {
        rotation.rotate_euler(radians);
    }

    Vector3 Transform::to_local(const Vector3 &global) const
    {
        Vector3 local = global;
        local -= origin;
        local = rotation.xform(local);
        local /= scale;
        return local;
    }

    Vector3 Transform::to_global(const Vector3 &local) const
    {
        Vector3 global = local;
        global = rotation.conjugate().xform(global);
        global *= scale;
        global += origin;
        return global;
    }
    #pragma endregion
}
