#ifndef BIGROCK_CELL_VERTEX_H
#define BIGROCK_CELL_VERTEX_H

#include "../../math/vector3.hpp"
#include "../../defines.hpp"

namespace bigrock {
namespace terrain {
namespace data {

struct CellVertex
{
    CellVertex(const math::Vector3 &position = math::Vector3::zero, const br_real &isovalue = -1.0, const unsigned int &material = 0);

    math::Vector3 position;
    br_real isovalue;
    unsigned int material;

    CellVertex interpolate(const CellVertex &other, const float &t) const;
    bool can_collapse(const CellVertex &other) const;
};

}}}

#endif
