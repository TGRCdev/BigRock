#ifndef BIGROCK_MATHFUNCS_H
#define BIGROCK_MATHFUNCS_H

#include <cmath>

namespace bigrock {
    template<typename T>
    T lerp(T begin, T end, float amount)
    {
        if(amount <= 0.0)
            return begin;
        else if(amount >= 1.0)
            return end;
        else
            return begin + ((end - begin) * amount);
    }

    template<typename T>
    bool sign_changes(T first, T second)
    {
        return (first ^ second) < 0;
    }

    template<>
    bool sign_changes<float>(float first, float second)
    {
        return std::signbit(first) != std::signbit(second);
    }

    template<>
    bool sign_changes<double>(double first, double second)
    {
        return std::signbit(first) != std::signbit(second);
    }

    template<>
    bool sign_changes<long double>(long double first, long double second)
    {
        return std::signbit(first) != std::signbit(second);
    }

    template<typename T>
    T abs(T value)
    {
        if(value > 0) return value;
        else return -value;
    }
}

#endif