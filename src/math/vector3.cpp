#include "vector3.hpp"

namespace bigrock
{
    namespace math
    {
        std::size_t hash_value(const bigrock::math::Vector3 &vec)
        {
            std::size_t h1 = boost::hash<br_real>()(vec.x);
            std::size_t h2 = boost::hash<br_real>()(vec.y);
            std::size_t h3 = boost::hash<br_real>()(vec.z);

            return h1 ^ h2 ^ h3; // TODO: Does this even work?
        }

        const Vector3 Vector3::zero = Vector3(0,0,0);
    }
}