#include "transform.hpp"
#include <glm/gtc/matrix_inverse.hpp>

namespace bigrock {
namespace data {
    Transform::Transform()
    {
        this->origin = Vector3(0.0f,0.0f,0.0f);
        this->rotation = glm::identity<Quat>();
        this->scale = Vector3(1.0f,1.0f,1.0f);
    }

    Transform::Transform(Vector3 origin)
    {
        this->origin = origin;
        this->rotation = glm::identity<Quat>();
        this->scale = Vector3(1.0f,1.0f,1.0f);
    }

    Transform::Transform(Vector3 origin, Quat rotation)
    {
        this->origin = origin;
        this->rotation = rotation;
        this->scale = Vector3(1.0f,1.0f,1.0f);
    }

    Transform::Transform(Vector3 origin, Quat rotation, Vector3 scale)
    {
        this->origin = origin;
        this->rotation = rotation;
        this->scale = scale;
    }

    Transform::Transform(const Transform &other)
    {
        this->origin = other.origin;
        this->rotation = other.rotation;
        this->scale = other.scale;
    }

    Transform::Transform(const schemas::Transformf *trns) : Transform()
    {
        if(trns)
        {
            this->origin = Vector3(trns->origin().x(), trns->origin().y(), trns->origin().z());
            this->rotation = Quat(trns->rotation().x(), trns->rotation().y(), trns->rotation().z(), trns->rotation().w());
            this->scale = Vector3(trns->scale().x(), trns->scale().y(), trns->scale().z());
        }
    }

    Transform::Transform(const schemas::Transformd *trns) : Transform()
    {
        if(trns)
        {
            this->origin = Vector3(trns->origin().x(), trns->origin().y(), trns->origin().z());
            this->rotation = Quat(trns->rotation().x(), trns->rotation().y(), trns->rotation().z(), trns->rotation().w());
            this->scale = Vector3(trns->scale().x(), trns->scale().y(), trns->scale().z());
        }
    }

    Vector3 Transform::to_local(Vector3 point) const
    {
        return glm::inverse(Mat4(*this)) * glm::vec4(point, 1.0f);
    }

    Vector3 Transform::to_global(Vector3 point) const
    {
        return Mat4(*this) * glm::vec4(point, 1.0f);
    }

    Transform::operator Mat4() const
    {
        return glm::scale(glm::translate(glm::identity<Mat4>(), origin) * glm::toMat4<br_real_t>(glm::normalize(rotation)), scale);
    }

    Transform::operator schemas::Transformf() const
    {
        return schemas::Transformf(
            schemas::Vec3f(origin.x, origin.y, origin.z),
            schemas::Vec4f(rotation.x, rotation.y, rotation.z, rotation.w),
            schemas::Vec3f(scale.x, scale.y, scale.z)
        );
    }

    Transform::operator schemas::Transformd() const
    {
        return schemas::Transformd(
            schemas::Vec3d(origin.x, origin.y, origin.z),
            schemas::Vec4d(rotation.x, rotation.y, rotation.z, rotation.w),
            schemas::Vec3d(scale.x, scale.y, scale.z)
        );
    }

    const Transform Transform::identity = Transform();
}}