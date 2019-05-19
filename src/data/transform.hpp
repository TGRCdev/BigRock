#ifndef BIGROCK_TRANSFORM_H
#define BIGROCK_TRANSFORM_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace bigrock {
namespace data {
    struct Transform
    {
        Transform();
        Transform(glm::vec3 origin);
        Transform(glm::vec3 origin, glm::quat rotation);
        Transform(glm::vec3 origin, glm::quat rotation, glm::vec3 scale);
        Transform(const Transform &other);

        glm::vec3 origin;
        glm::quat rotation;
        glm::vec3 scale;
        
        glm::vec3 to_global(glm::vec3 point) const;
        glm::vec3 to_local(glm::vec3 point) const;

        operator glm::mat4() const;

        static const Transform identity;
    };
}}

#endif