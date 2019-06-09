#ifndef BIGROCK_MATH_FUNCS
#define BIGROCK_MATH_FUNCS
#pragma once

#include <limits>
#include <glm/vec3.hpp>
#include <functional>

namespace bigrock
{
    inline bool sign_changes(float val1, float val2)
    {
        return (val1 * val2) < 0;
    }

    inline bool approx_equal(float val1, float val2)
    {
        return std::abs(val1 - val2) < std::numeric_limits<float>::epsilon();
    }
}

namespace std
{
    template<>
    struct less<glm::vec3>
    {
        bool operator() (const glm::vec3 &lh, const glm::vec3 &rh) const
        {
            if(lh.x < rh.x)
                return true;
            else if(lh.x > rh.x)
                return false;
            else
            {
                if(lh.y < rh.y)
                    return true;
                else if(lh.y > rh.y)
                    return false;
                else
                {
                    if(lh.z < rh.z)
                        return true;
                    else
                        return false;
                }
            }
        }
    };
}

#endif