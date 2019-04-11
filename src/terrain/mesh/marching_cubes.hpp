#ifndef BIGROCK_MARCHING_CUBES_H
#define BIGROCK_MARCHING_CUBES_H

#include "polygoniser.hpp"
#include <limits>

namespace bigrock {
namespace terrain {
namespace mesh {

class MarchingCubes : public Polygoniser
{
    Mesh polygonise(const data::Cell &cell, const unsigned int &max_depth = UINT_MAX) const;
};

}}}

#endif