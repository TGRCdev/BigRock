#ifndef BIGROCK_TERRAIN_MESHER_H
#define BIGROCK_TERRAIN_MESHER_H
#pragma once

#include "../defines.hpp"
#include "../data/cell.hpp"
#include "mesh.hpp"
#include <memory>

namespace bigrock {
namespace mesh {
using namespace data;

// Extended for various isosurface generating algorithms
class TerrainMesher
{
    public:
    virtual std::unique_ptr<Mesh> generate(const Cell &cell, const unsigned char max_depth = BR_MAX_CELL_DEPTH) const = 0;
};

}}

#endif