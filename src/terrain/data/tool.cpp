#include "tool.hpp"
#include "grid_vertices.hpp"

#include <math.h>

namespace bigrock
{
    namespace terrain
    {
        unsigned int Tool::get_min_subdiv() const
        {
            return floor(1.0 / fmin(transform.scale.x, fmin(transform.scale.y, transform.scale.z)));
        }

        unsigned int Tool::get_max_subdiv() const
        {
            return 8;
        }

        br_real EllipsoidTool::value(const math::Vector3 &point) const
        {
            br_real dist_sqr = transform.to_local(point).length_squared();

            if(dist_sqr >= 1)
                return 0;
            else
                return 3 * (1 - ((22.0/9.0) * dist_sqr) + ((17.0/9.0) * (dist_sqr * dist_sqr)) - ((4.0/9.0) * (dist_sqr * dist_sqr * dist_sqr)));
        }

        math::AABB EllipsoidTool::get_aabb(const math::Transform &xform) const
        {
            math::Vector3 minpos = xform.to_local(transform.to_global(GRID_VERTICES[0]));
            math::Vector3 maxpos = minpos;

            for(int i = 1; i < 8; i++)
            {
                math::Vector3 pos = xform.to_local(transform.to_global(GRID_VERTICES[i]));

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
                else if(pos.z > minpos.z)
                    maxpos.z = pos.z;
            }
            return math::AABB(minpos, maxpos - minpos);
        }
    }
}
