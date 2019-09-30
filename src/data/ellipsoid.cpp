#include "ellipsoid.hpp"

namespace bigrock
{
    isovalue_t Ellipsoid::get_value_local(glm::vec3 local) const
    {
        float dist_sqr = glm::length2(local);
        if (dist_sqr >= 1.0)
            return -1;
        else
            return (1.0 - ((22.0/9.0) * dist_sqr) + ((17.0/9.0) * (dist_sqr * dist_sqr)) - ((4.0/9.0) * (dist_sqr * dist_sqr * dist_sqr)));
            // TODO: Dig through my research and find the origin of these magic numbers
    }

    AABB Ellipsoid::get_surface_aabb() const
    {
        AABB ret;
        glm::vec3 points[8] = {glm::vec3(-1,-1,-1), glm::vec3(1,-1,-1), glm::vec3(-1,1,-1), glm::vec3(1,1,-1), glm::vec3(-1,-1,1), glm::vec3(1,-1,1), glm::vec3(-1,1,1), glm::vec3(1,1,1)};
        for(int i = 0; i < 8; i++)
            ret.extend(transform.to_global(points[i]));
        return ret;
    }

    AABB Ellipsoid::get_tool_aabb() const
    {
        AABB ret;
        glm::vec3 points[8] = {glm::vec3(-2,-2,-2), glm::vec3(2,-2,-2), glm::vec3(-2,2,-2), glm::vec3(2,2,-2), glm::vec3(-2,-2,2), glm::vec3(2,-2,2), glm::vec3(-2,2,2), glm::vec3(2,2,2)};
        for(int i = 0; i < 8; i++)
            ret.extend(transform.to_global(points[i]));
        return ret;
    }
}