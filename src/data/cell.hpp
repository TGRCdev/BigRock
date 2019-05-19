#ifndef BIGROCK_CELL_H
#define BIGROCK_CELL_H
#pragma once

#include "point.hpp"
#include "tool.hpp"
#include "action.hpp"
#include "query_structs.hpp"

namespace bigrock {
namespace data {

class Cell
{
    Point *corners[8];
    Cell *children; // Cell is a leaf if children in NULL, else children is size 8.
    char owned_vertices; // Bit field describing which vertices this cell can delete.
    unsigned char subdiv_level; // Any subdivision level above 10 is total overkill

    public:
    Cell(bool init_root=true); // If init_root, corners are created and owned by the cell.
    ~Cell(); // Frees points described by owned_vertices

    bool is_leaf() const {return children == NULL;}

    bool has_children() const {return children != NULL;}

    /// Splits the cell into 8 child cells. If the cell already has children, does nothing.
    void subdivide();

    /// Collapses the cell's children. If the cell is a leaf, does nothing.
    void undivide();

    /// Interpolates the corners using the global point given
    Point sample(glm::vec3 point) const;
    /// Interpolates the corners using the local point with values [0-1] given
    Point sample_local(glm::vec3 point) const;

    ToolQueryResult apply(const Tool &t, const Action &a);
};

}}

#endif