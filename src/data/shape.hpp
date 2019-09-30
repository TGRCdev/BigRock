#ifndef BIGROCK_SHAPE_H
#define BIGROCK_SHAPE_H
#pragma once

#include "transform.hpp"
#include "../defines.hpp"
#include "AABB.hpp"

namespace bigrock
{
    /// Abstract class representing a CSG shape tool.
    struct Shape
    {
        Transform transform;

        // Returns the isovalue of the given global coordinate point in the Shape tool.
        virtual isovalue_t get_value(glm::vec3 point) const {return get_value_local(transform.to_local(point));}

        // Returns the isovalue of the given local coordinate local in the Shape tool.
        virtual isovalue_t get_value_local(glm::vec3 local) const = 0;

        // Returns the AABB containing the surface of the tool
        virtual AABB get_surface_aabb() const = 0;

        /// Returns the AABB containing the complete area of influence for the tool,
        /// including space outside of the shape's surface that has an isovalue > -1
        virtual AABB get_tool_aabb() const = 0;

        // TODO: Serialization, Deserialization
    };
}

#endif