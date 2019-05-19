#include "transform.hpp"
#include <glm/gtc/matrix_inverse.hpp>

namespace bigrock {
namespace data {
    Transform::Transform()
    {
        this->origin = glm::vec3(0.0f,0.0f,0.0f);
        this->rotation = glm::identity<glm::quat>();
        this->scale = glm::vec3(1.0f,1.0f,1.0f);
    }

    Transform::Transform(glm::vec3 origin)
    {
        this->origin = origin;
        this->rotation = glm::identity<glm::quat>();
        this->scale = glm::vec3(1.0f,1.0f,1.0f);
    }

    Transform::Transform(glm::vec3 origin, glm::quat rotation)
    {
        this->origin = origin;
        this->rotation = rotation;
        this->scale = glm::vec3(1.0f,1.0f,1.0f);
    }

    Transform::Transform(glm::vec3 origin, glm::quat rotation, glm::vec3 scale)
    {
        this->origin = origin;
        this->rotation = rotation;
        this->scale = scale;
    }

    Transform::Transform(const Transform &other)
    {
        this->origin = other.origin;
        this->rotation = other.origin;
        this->scale = other.scale;
    }

    glm::vec3 Transform::to_local(glm::vec3 point) const
    {
        return glm::inverse(glm::mat4(*this)) * glm::vec4(point, 1.0f);
    }

    glm::vec3 Transform::to_global(glm::vec3 point) const
    {
        return glm::mat4(*this) * glm::vec4(point, 1.0f);
    }

    Transform::operator glm::mat4() const
    {
        return glm::scale(glm::translate(glm::identity<glm::mat4>(), origin) * glm::toMat4(glm::normalize(rotation)), scale);
    }

    const Transform Transform::identity = Transform();
}}