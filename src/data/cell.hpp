#ifndef BIGROCK_CELL_H
#define BIGROCK_CELL_H
#pragma once

#include "point.hpp"
#include "tool.hpp"
#include "action.hpp"
#include "query_structs.hpp"

#include "schemas/cell_generated.h"

namespace bigrock {
namespace data {

class Cell
{
    Point *corners[8];
    Cell *children; // Cell is a leaf if children in NULL, else children is size 8.
    char owned_vertices; // Bit field describing which vertices this cell can delete.
    unsigned char subdiv_level;

    unsigned int *cell_count; // Pointer to the amount of cells in this tree of Cells. Passed by parent.

    flatbuffers::Offset<schemas::Cell> serialize(flatbuffers::FlatBufferBuilder &builder, std::map<const Point*, flatbuffers::Offset<schemas::Point> > &point_offsets) const;

    typedef std::map<const schemas::Point*, Point*> PointMap;

    void load(const schemas::Cell &cell, PointMap &points);
    Cell(const schemas::Cell &Cell, PointMap &points, unsigned int *cell_count);
    Cell(const schemas::Cell &cell);

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

    const Point &get_corner(int index) {return *corners[index];}
    unsigned char get_depth() {return subdiv_level;}
    Cell *get_child(int index) {return (has_children() ? &children[index] : NULL);}

    std::unique_ptr<std::string> serialize() const;
    static std::unique_ptr<Cell> deserialize(const void *buf, size_t length);
};

}}

#endif