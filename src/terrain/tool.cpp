#include "tool.h"
#include "../defines.hpp"

namespace bigrock
{
    namespace terrain
    {
        br_real EllipsoidTool::value(const math::Vector3 &point) const
        {
            br_real dist_sqr = transform.to_local(point).length_squared(); // Distance squared to origin

            if(dist_sqr >= 1)
            	return 0;
            else // Wyvill's field function, according to Resolution Adaptive Volume Sculpting
            	return 3 * (1 - ((22.0/9.0)*dist_sqr) + ((17.0/9.0)*(dist_sqr*dist_sqr)-((4.0/9.0)*(dist_sqr*dist_sqr*dist_sqr))));
        }

        unsigned int EllipsoidTool::get_min_subdiv(const math::Vector3 &point) const
        {
            return br_tool_min_subdiv;
        }

        unsigned int EllipsoidTool::get_max_subdiv(const math::Vector3 &point) const
        {
            return br_tool_max_subdiv;
        }
    }
}
