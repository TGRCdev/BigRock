#include "tool.hpp"

namespace bigrock {
namespace terrain {
namespace data {

ToolCopy::ToolCopy(const Tool *tool_copy, const math::Transform &t) : tool_copy(tool_copy)
{
    this->transform = t;
}

ToolCopy::ToolCopy(const ToolCopy &other) : tool_copy(other.tool_copy)
{
    this->transform = other.transform;
}

ToolCopy::~ToolCopy()
{
    delete tool_copy;
}

br_real ToolCopy::value(const math::Vector3 &point) const
{
    return tool_copy->value(transform.to_local(point));
}

math::AABB ToolCopy::get_aabb() const
{
    return tool_copy->get_aabb();
}

ToolCopy ToolCopy::get_copy() const
{ // PLEASE DONT EVER DO THIS
    return ToolCopy(*this);
}

}
}
}