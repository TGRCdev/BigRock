#ifndef BIGROCK_TRANSFORM_H
#define BIGROCK_TRANSFORM_H
#pragma once

#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

namespace bigrock {

    /// Basic Transform structure containing some convenience methods.
    struct Transform
    {
        glm::vec3 origin;
        glm::quat rotation;
        glm::vec3 scale;

        Transform(
            glm::vec3 origin = glm::zero<glm::vec3>(), 
            glm::quat rotation = glm::identity<glm::quat>(), 
            glm::vec3 scale = glm::one<glm::vec3>()
            ) :
        origin(origin),
        rotation(rotation),
        scale(scale) {}

        // Returns the inverse transformation
        Transform inverse() const;
        // Inverts the Transform
        void invert();

        // Returns a Transform with the rotation normalized
        Transform normalized() const;
        // Normalizes the rotation
        void normalize();

        // Transforms the given point using this Transform
        glm::vec3 xform(glm::vec3 point) const;
        // Transforms the given point inversely using this Transform
        glm::vec3 xform_inv(glm::vec3 point) const;

        /// Transforms the local coordinate point of the Transform to global coordinates
        /// Alias of xform
        glm::vec3 to_global(glm::vec3 local) const {return xform(local);}
        /// Transforms the global coordinate point to local coordinates of the Transform
        /// Alias of xform_inv
        glm::vec3 to_local(glm::vec3 global) const {return xform_inv(global);}

        // TODO: Serialization, Deserialization
    };

}

#endif