#include "ellipsoid_tool.hpp"

namespace bigrock {
namespace terrain {
namespace data {

EllipsoidTool::EllipsoidTool(const math::Transform &t)
{
    this->transform = t;
}

br_real EllipsoidTool::value(const math::Vector3 &pos) const
{
    br_real dist_sqr = transform.to_local(pos).length_squared();
    if (dist_sqr >= 1.0)
        return -1;
    else
        return (2.0 * (1.0 - ((22.0/9.0) * dist_sqr) + ((17.0/9.0) * (dist_sqr * dist_sqr)) - ((4.0/9.0) * (dist_sqr * dist_sqr * dist_sqr)))) - 1.0;
}

math::AABB EllipsoidTool::get_aabb() const
{
    math::Vector3 grid[8] = {math::Vector3(-1, -1, -1), math::Vector3(1, -1, -1), math::Vector3(-1, 1, -1), math::Vector3(1, 1, -1), math::Vector3(-1, -1, 1), math::Vector3(1, -1, 1), math::Vector3(-1, 1, 1), math::Vector3(1,1,1)};
    math::Vector3 minpos = transform.to_global(grid[0]);
    math::Vector3 maxpos = minpos;

    for(int i = 1; i < 8; i++)
    {
        math::Vector3 pos = transform.to_global(grid[i]);
        if(pos.x < minpos.x)
            minpos.x = pos.x;
        else if(pos.x > maxpos.x)
            maxpos.x = pos.x;

        if(pos.y < minpos.y)
            minpos.y = pos.y;
        else if(pos.y > maxpos.y)
            maxpos.y = pos.y;

        if(pos.z < minpos.z)
            minpos.z = pos.z;
        else if(pos.z > maxpos.z)
            maxpos.z = pos.z;
    }
    math::AABB ret(minpos);
    ret.size = maxpos - minpos;
    return ret;
}

ToolCopy EllipsoidTool::get_copy() const
{
    return ToolCopy(new EllipsoidTool(*this));
}

}}}
