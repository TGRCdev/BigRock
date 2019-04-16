#include "cell_vertex.hpp"

#include "../../math/math_funcs.hpp"

namespace bigrock {
namespace terrain {
namespace data {

CellVertex::CellVertex(const math::Vector3 &position, const br_real &value, const unsigned int &material)
{
    this->position = position;
    this->isovalue = value;
    this->material = material;
}

CellVertex CellVertex::interpolate(const CellVertex &other, const float &t) const
{
    if (t <= 0.0)
        return *this;
    else if(t >= 1.0)
        return other;
    else
    {
        CellVertex ret;
        if(this->material == other.material)
        {
            ret.material = this->material;
            ret.isovalue = math::lerp(this->isovalue, other.isovalue, t);
            ret.position = this->position.lerp(other.position, t);
        }
        else
        {
            br_real begin = abs(this->isovalue);
            br_real end = -abs(other.isovalue);
            br_real mat_val = math::lerp(begin, end, t);
            if(mat_val < 0)
                ret.material = other.material;
            else
                ret.material = this->material;

            ret.isovalue = math::lerp(this->isovalue, other.isovalue, t);
            ret.position = this->position.lerp(other.position, t);
        }
        return ret;
    }
}

bool CellVertex::can_collapse(const CellVertex &other) const
{
    if(math::sign_changes(this->isovalue, other.isovalue)) // Crosses isosurface
        return false;
    else if(this->isovalue <= 0 && other.isovalue <= 0) // Empty space
        return true;
    else if(this->material != other.material) // Filled space, but seperate materials
        return false;
    else // Filled space with same material
        return true;
}

}}}
