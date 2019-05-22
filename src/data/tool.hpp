#ifndef BIGROCK_TOOL_H
#define BIGROCK_TOOL_H
#pragma once

#include "transform.hpp"
#include "AABB.hpp"
#include "serialize.hpp"

namespace bigrock {
namespace data {

/// CSG tool for modifying terrain
struct Tool
{
    /// When extending, keep in mind:
    /// - Default constructor
    /// - Serialize function
    /// - Deserialize function
    /// - value_local
    /// - get_aabb
    /// - get_tool_type

    Transform transform;

    /// Returns the density of the point in the tool transformed by the tool's transform
    float value(glm::vec3 point) const;

    /// Returns the density of the point in the tool without being transformed
    virtual float value_local(glm::vec3 point) const = 0;

    /// Gets the AABB of the tool, which represents the maximum possible area that the tool can affc
    virtual AABB get_aabb() const;

    enum ToolType
    {
        TOOL_TYPE_NULL = '\0',
        TOOL_TYPE_ELLIPSOID = 'E'
    };

    virtual inline ToolType get_tool_type() const {return TOOL_TYPE_NULL;}

    static Tool *new_tool(char tooltype);

    static Tool *deserialize_tool(const char *buffer, int *chars_read = NULL);

    /// Adds additional bytes describing the string as a tool, and what tool type it is.
    static int serialize_tool(const Tool &t, char *buffer, bool include_prefix = false);
};

}}

#endif