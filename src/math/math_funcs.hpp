#ifndef BIGROCK_MATH_FUNCS
#define BIGROCK_MATH_FUNCS

#define _USE_MATH_DEFINES
#include <math.h>
#include <limits>

namespace bigrock
{
    namespace math
    {
        static float degrees_to_radians(const float degrees)
        {
            return degrees * (M_PI / 180);
        }

        static float radians_to_degrees(const float radians)
        {
            return radians * (180 / M_PI);
        }

        static float lerp(const float v1, const float v2, const float t)
        {
            return v1 + ((v2 - v1) * t);
        }

        static float approx_equal(const float v1, const float v2)
        {
            float epsilon = std::numeric_limits<float>::epsilon();
            return fabs(v2 - v1) < epsilon;
        }
    }
}

#endif
