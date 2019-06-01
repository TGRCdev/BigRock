#include "point.hpp"

#include <glm/common.hpp>

#include "../math_funcs.hpp"

namespace bigrock {
namespace data {

Point::Point()
{
    this->isovalue = -1.0f;
    this->material = 0;
}

Point::Point(const schemas::Point &point)
{
    this->isovalue = point.isovalue();
    this->material = point.material();
}

Point Point::interpolate(const Point &other, const float &t) const
{
    if(t <= 0)
        return *this;
    else if(t >= 1)
        return other;
    else
    {
        Point ret;
        ret.isovalue = glm::mix(this->isovalue, other.isovalue, t);
        if(this->material == other.material)
            ret.material = this->material;
        else
        {
            float begin = abs(this->isovalue);
            float end = -abs(other.isovalue);
            float matval = glm::mix(begin, end, t);
            if(matval <= 0)
                ret.material = other.material;
            else
                ret.material = this->material;
        }
        return ret;
    }
}

bool Point::can_collapse(const Point &other) const
{
    if(sign_changes(this->isovalue, other.isovalue)) // Crosses isosurface
        return false;
    else if(this->isovalue >= 0 && this->material != other.material) // Filled with different materials
        return false;
    else // Either both empty, or filled with same material
        return true;
}

}}