#ifndef BIGROCK_ELLIPSOID_H
#define BIGROCK_ELLIPSOID_H
#pragma once

#include "../shape.hpp"

namespace bigrock {

    /// Ellipsoid CSG tool
    ///
    /// The scale of the ellipsoid can be considered the Ellipsoid's radius for each axis.
    /// For method definitions, see shape.hpp
    struct Ellipsoid : public Shape
    {
        isovalue_t get_value_local(glm::vec3 local) const;

        AABB get_surface_aabb() const;
        AABB get_tool_aabb() const;

        std::unique_ptr<Shape> duplicate() const;

        // TODO: Serialization, Deserialization
    };

}

#endif