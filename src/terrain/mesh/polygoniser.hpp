#ifndef BIGROCK_POLYGONISER_H
#define BIGROCK_POLYGONISER_H

#include "../data/cell.hpp"
#include "vertex.hpp"
#include "mesh.hpp"

#include <limits>

namespace bigrock {
namespace terrain {
namespace mesh {

struct Polygoniser
{
    /*
     * Polygonises the cell down to max_depth.
     * If max_depth is set to 0, it will polygonise until no more children are found.
     */
    virtual Mesh polygonise(const data::Cell &cell, const unsigned int &max_depth = UINT_MAX) const = 0;
};

}
}
}

#endif