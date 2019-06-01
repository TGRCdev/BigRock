#ifndef BIGROCK_TRANSFORM_H
#define BIGROCK_TRANSFORM_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "schemas/common_generated.h"
#include "../defines.hpp"

namespace bigrock {
namespace data {
    struct Transform
    {
        Transform();
        Transform(Vector3 origin);
        Transform(Vector3 origin, Quat rotation);
        Transform(Vector3 origin, Quat rotation, Vector3 scale);
        Transform(const Transform &other);

        Transform(const schemas::Transformf *trns);
        Transform(const schemas::Transformd *trns);

        Vector3 origin;
        Quat rotation;
        Vector3 scale;
        
        Vector3 to_global(Vector3 point) const;
        Vector3 to_local(Vector3 point) const;

        operator Mat4() const;
        operator schemas::Transformf() const;
        operator schemas::Transformd() const;

        static const Transform identity;
    };
}}

#endif