#ifndef BIGROCK_VECTOR3I_H
#define BIGROCK_VECTOR3I_H

#include "vector3.hpp"

namespace bigrock
{
    namespace math
    {
        struct Vector3i
        {
            int x, y, z;

            Vector3i();
            Vector3i(int x, int y, int z);
            Vector3i(const Vector3i &other);
            Vector3i(const Vector3 &other);

            bool operator==(const Vector3i &other) const;
            bool operator!=(const Vector3i &other) const;

            Vector3i operator+(const Vector3i &other) const;
            Vector3i &operator+=(const Vector3i &other);
            Vector3i operator-(const Vector3i &other) const;
            Vector3i &operator-=(const Vector3i &other);
            // TODO: add all of these operators

            Vector3i operator%(const int val) const;
            Vector3i &operator%=(const int val);
            
            operator Vector3() const;
        };
    }
}

namespace std
{
    template<>
    struct less<bigrock::math::Vector3i> // This is for use in std::map
    {
        bool operator() (const bigrock::math::Vector3i &lhs, const bigrock::math::Vector3i &rhs) const
        {
            if(lhs.x < rhs.x)
                return true;
            else if(lhs.x > rhs.x)
                return false;
            else
            {
                if(lhs.y < rhs.y)
                    return true;
                else if(lhs.y > rhs.y)
                    return false;
                else
                    return lhs.z < rhs.z;
            }
        }
    };
}

#endif