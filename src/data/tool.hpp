#ifndef BIGROCK_TOOL_H
#define BIGROCK_TOOL_H
#pragma once

#include "transform.hpp"
#include "AABB.hpp"

#include <string>
#include <memory>

namespace bigrock {
namespace data {

/// CSG tool for modifying terrain
struct Tool
{
    Transform transform;

    /// Returns the density of the point in the tool transformed by the tool's transform
    float value(Vector3 point) const;

    /// Returns the density of the point in the tool without being transformed
    virtual float value_local(Vector3 point) const = 0;

    /// Gets the AABB of the tool, which represents the maximum possible area that the tool can affc
    virtual AABB get_aabb() const;

    enum ToolType
    {
        TOOL_TYPE_ELLIPSOID
    };

    virtual ToolType get_tool_type() const = 0;

    /// Returns true if the tool is concave, false if convex.
    /// If a tool is concave, cell MUST subdivide to the lowest
    /// level to reveal ALL detail
    virtual bool is_concave() const = 0;

    /// Returns the maximum subdivision depth to get all detail
    /// Returns -1 if max depth is not applicable, i.e. if the tool
    /// is convex
    virtual int get_max_depth() const = 0;

    /// Returns a UTF-8 string with the encoded object.
    /// Returns an empty string on error.
    std::string serialize() const;

    /// Returns the Tool stored in the buffer
    /// Returns nullptr if the buffer is invalid
    static std::unique_ptr<Tool> deserialize(const void *buf, size_t length);
};

}}

#endif