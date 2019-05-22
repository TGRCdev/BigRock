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

std::list<Member<Ellipsoid> > Ellipsoid::members = std::list<Member<Ellipsoid> >();

}

#define offset(class, attrib) reinterpret_cast<intptr_t>(&((class*)0)->attrib) // Is it safe? Who knows, but it shuts MinGW up

using namespace tools;
template<>
std::list<Member<Ellipsoid> > get_member_list()
{
    if(Ellipsoid::members.empty())
    {
        Ellipsoid::members.push_back(Member<Ellipsoid>(SERIAL_TYPE_FLOAT, offset(Ellipsoid, transform.origin.x)));
        Ellipsoid::members.push_back(Member<Ellipsoid>(SERIAL_TYPE_FLOAT, offset(Ellipsoid, transform.origin.y)));
        Ellipsoid::members.push_back(Member<Ellipsoid>(SERIAL_TYPE_FLOAT, offset(Ellipsoid, transform.origin.z)));
        Ellipsoid::members.push_back(Member<Ellipsoid>(SERIAL_TYPE_FLOAT, offset(Ellipsoid, transform.rotation.x)));
        Ellipsoid::members.push_back(Member<Ellipsoid>(SERIAL_TYPE_FLOAT, offset(Ellipsoid, transform.rotation.y)));
        Ellipsoid::members.push_back(Member<Ellipsoid>(SERIAL_TYPE_FLOAT, offset(Ellipsoid, transform.rotation.z)));
        Ellipsoid::members.push_back(Member<Ellipsoid>(SERIAL_TYPE_FLOAT, offset(Ellipsoid, transform.rotation.w)));
        Ellipsoid::members.push_back(Member<Ellipsoid>(SERIAL_TYPE_FLOAT, offset(Ellipsoid, transform.scale.x)));
        Ellipsoid::members.push_back(Member<Ellipsoid>(SERIAL_TYPE_FLOAT, offset(Ellipsoid, transform.scale.y)));
        Ellipsoid::members.push_back(Member<Ellipsoid>(SERIAL_TYPE_FLOAT, offset(Ellipsoid, transform.scale.z)));
    }
    return Ellipsoid::members;
}

}}