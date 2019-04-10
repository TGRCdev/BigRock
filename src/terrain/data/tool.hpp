#ifndef BIGROCK_TOOL_H
#define BIGROCK_TOOL_H

#include "../../math/transform.hpp"
#include "../../defines.hpp"
#include "../../math/vector3.hpp"
#include "../../math/aabb.hpp"

namespace bigrock {
namespace terrain {
namespace data {

struct ToolCopy;

struct Tool
{
    math::Transform transform;

    // This should be deterministic, only changing when transform is changed.
    virtual br_real value(const math::Vector3 &point) const = 0;
    virtual math::AABB get_aabb() const = 0;
    virtual ToolCopy get_copy() const = 0;
};

// Contains a freezed copy of the tool for multi-threaded applying
struct ToolCopy : public Tool
{ // Please never create these on your own, use get_copy() on any tool
    ToolCopy(const Tool *tool_copy, const math::Transform &t = math::Transform::identity);
    ToolCopy(const ToolCopy &other);
    ~ToolCopy();

    const Tool *tool_copy;

    br_real value(const math::Vector3 &point) const;
    math::AABB get_aabb() const;
    // DON'T CALL THIS ON TOOLCOPY PLEASE
    ToolCopy get_copy() const;
};

}}}

#endif