#include "ellipsoid.hpp"
#include "../../math_funcs.hpp"

#if !(defined(_WIN32) || defined(_WIN64))
#include <string.h> // memcpy
#endif

namespace bigrock {
namespace data {
namespace tools {

float Ellipsoid::value_local(glm::vec3 point) const
{
    // return (2.0 * (1.0 - ((22.0/9.0) * dist_sqr) + ((17.0/9.0) * (dist_sqr * dist_sqr)) - ((4.0/9.0) * (dist_sqr * dist_sqr * dist_sqr)))) - 1.0;
    float dist_sqr = glm::length2(point);
    if(dist_sqr >= 1.0)
        return -1;
    else
        return (2.0 * (1.0 - ((22.0/9.0) * dist_sqr) + ((17.0/9.0) * (dist_sqr * dist_sqr)) - ((4.0/9.0) * (dist_sqr * dist_sqr * dist_sqr)))) - 1.0;
}

std::list<Member<Ellipsoid>> Ellipsoid::members = std::list<Member<Ellipsoid>>();

}

using namespace tools;
template<>
std::list<Member<Ellipsoid>> get_member_list()
{
    if(Ellipsoid::members.empty())
    {
        Ellipsoid::members.push_back(Member<Ellipsoid>(SERIAL_TYPE_FLOAT, offsetof(Ellipsoid, transform.origin.x)));
        Ellipsoid::members.push_back(Member<Ellipsoid>(SERIAL_TYPE_FLOAT, offsetof(Ellipsoid, transform.origin.y)));
        Ellipsoid::members.push_back(Member<Ellipsoid>(SERIAL_TYPE_FLOAT, offsetof(Ellipsoid, transform.origin.z)));
    }
    return Ellipsoid::members;
}

}}