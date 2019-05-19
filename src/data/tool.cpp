#include "tool.hpp"
#include "../grid_vertices.hpp"
#include "serialize.hpp"

// Tools
#include "tools/ellipsoid.hpp"

namespace bigrock {
namespace data {

float Tool::value(glm::vec3 point) const
{
    return value_local(transform.to_local(point));
}

Tool *Tool::new_tool(char tooltype)
{
    switch(tooltype)
    {
        case TOOL_TYPE_ELLIPSOID:
            return new tools::Ellipsoid();
        default:
            return NULL;
    }
}

AABB Tool::get_aabb() const
{
    AABB ret;
    for(int i = 0; i < 8; i++)
        ret.extend(GRID_VERTICES[i] - glm::vec3(0.5f));
    return ret;
}

}}