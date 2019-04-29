#ifndef BIGROCK_MATH_FUNCS
#define BIGROCK_MATH_FUNCS
#pragma once

#include <limits>

namespace bigrock
{
    inline bool sign_changes(float val1, float val2)
    {
        return (val1 * val2) < 0;
    }

    inline bool approx_equal(float val1, float val2)
    {
        return (val1 - val2) < std::numeric_limits<float>::epsilon();
    }
}

#endif