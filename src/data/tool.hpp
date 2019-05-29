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
    float value(glm::vec3 point) const;

    /// Returns the density of the point in the tool without being transformed
    virtual float value_local(glm::vec3 point) const = 0;

    /// Gets the AABB of the tool, which represents the maximum possible area that the tool can affc
    virtual AABB get_aabb() const;

    enum ToolType
    {
        TOOL_TYPE_ELLIPSOID
    };

    virtual ToolType get_tool_type() const = 0;

    /// Returns a UTF-8 string with the encoded object.
    /// Returns an empty string on error.
    std::string serialize() const;

    static std::unique_ptr<Tool> deserialize(const void *buf, size_t length);
};

}}

#endif