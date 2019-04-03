#ifndef BIGROCK_MATH_FUNCS
#define BIGROCK_MATH_FUNCS

#define _USE_MATH_DEFINES

#include <math.h>
#include <limits>

#include "../defines.hpp"

namespace bigrock
{
    namespace math
    {
        static br_real degrees_to_radians(const br_real degrees)
        {
            return degrees * (M_PI / 180);
        }

        static br_real radians_to_degrees(const br_real radians)
        {
            return radians * (180 / M_PI);
        }

        static br_real lerp(const br_real v1, const br_real v2, const br_real t)
        {
            return v1 + ((v2 - v1) * t);
        }

        static br_real approx_equal(const br_real v1, const br_real v2)
        {
            if(v1 == v2)
                return true;
            
            br_real epsilon = std::numeric_limits<br_real>::epsilon();
            return abs(v2 - v1) < epsilon;
        }
    }
}

#endif