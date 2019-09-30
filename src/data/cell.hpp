#ifndef BIGROCK_CELL_H
#define BIGROCK_CELL_H

#include "../defines.hpp"

#include "point.hpp"
#include "shape.hpp"

namespace bigrock {

/// A chunk of terrain, represented in local coordinates between (0,0,0) and (1,1,1)
/// Has pointers to 8 Points that represent the corners of the Cell.
class Cell
{
    Point *corners[8];
    bitflags8_t owned_corners;
    Cell *children;

public:
    
};

}

#endif