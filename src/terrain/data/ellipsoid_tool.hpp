#ifndef BIGROCK_ELLIPSOID_TOOL_H
#define BIGROCK_ELLIPSOID_TOOL_H

#include "tool.hpp"

namespace bigrock {
namespace terrain {
namespace data {

struct EllipsoidTool : public Tool
{
    EllipsoidTool(const math::Transform &t);

    br_real value(const math::Vector3 &point) const;
    math::AABB get_aabb() const;
    ToolCopy get_copy() const;
};

}}}

#endif
