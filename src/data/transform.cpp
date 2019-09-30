#include "transform.hpp"

namespace bigrock {

    void Transform::invert()
    {
        this->rotation = glm::inverse(rotation);
        this->scale = glm::one<glm::vec3>() / this->scale;
        this->origin = -origin;
    }

    Transform Transform::inverse() const
    {
        Transform ret = *this;
        ret.invert();
        return ret;
    }

    void Transform::normalize()
    {
        this->rotation = glm::normalize(rotation);
    }

    Transform Transform::normalized() const
    {
        Transform ret = *this;
        ret.normalize();
        return ret;
    }

    glm::vec3 Transform::xform(glm::vec3 point) const
    {
        glm::vec3 result = glm::toMat4(rotation) * glm::vec4(point, 1.0f); // Rotate
        result *= scale; // Scale
        result += origin; // Translate
        return result;
    }

    glm::vec3 Transform::xform_inv(glm::vec3 point) const
    {
        glm::vec3 result = point - origin; // Translate
        result = glm::toMat4(glm::inverse(rotation)) * glm::vec4(result, 1.0f); // Rotate
        result /= scale; // Scale
        return result;
    }
}