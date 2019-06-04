#ifndef BIGROCK_CELL_H
#define BIGROCK_CELL_H
#pragma once

#include "point.hpp"
#include "tool.hpp"
#include "action.hpp"
#include "query_structs.hpp"
#include "jobpool.hpp"

#include "../grid_vertices.hpp"

#include "schemas/cell_generated.h"

namespace bigrock {
namespace data {

class Cell
{
    Vector3 position;
    Point *corners[8];
    Cell *children; // Cell is a leaf if children in NULL, else children is size 8.
    char owned_vertices; // Bit field describing which vertices this cell can delete.
    unsigned char subdiv_level;

    unsigned int *cell_count; // Pointer to the amount of cells in this tree of Cells. Passed by parent.

    flatbuffers::Offset<
    #if BR_USE_DOUBLE_PRECISION
    schemas::CellDouble
    #else
    schemas::CellSingle
    #endif
    > serialize(flatbuffers::FlatBufferBuilder &builder, std::map<const Point*, flatbuffers::Offset<schemas::Point> > &point_offsets) const;

    typedef std::map<const schemas::Point*, Point*> PointMap;
    
    #if !BR_DISABLE_MULTITHREADING
    static int cell_apply_thread(void *userdata);
    void apply_threaded(const Tool &t, const Action &a, const int max_depth);
    #endif

    void apply_unthreaded(const Tool &t, const Action &a, const int max_depth);

    template<class T>
    void load(const T &cell, PointMap &points);
    template<class T>
    Cell(const T &Cell, PointMap &points, unsigned int *cell_count, unsigned char subdiv_level, Vector3 position);

    Cell(const schemas::CellRoot &cell);

    public:
    
    Cell(bool init_root=true); // If init_root, corners are created and owned by the cell.
    ~Cell(); // Frees points described by owned_vertices

    bool is_leaf() const {return children == NULL;}

    bool has_children() const {return children != NULL;}

    bool owns_corner(char index) const {return (owned_vertices & (1 << index)) != 0;}

    /// Splits the cell into 8 child cells. If the cell already has children, does nothing.
    void subdivide();

    /// Collapses the cell's children. If the cell is a leaf, does nothing.
    void undivide();

    /// Attempts to collapse similar cells to save memory and increase performance
    void optimize();

    /// Interpolates the corners using the global point given
    Point sample(Vector3 point, bool recursive = true) const;
    /// Interpolates the corners using the local point with values [0-1] given
    Point sample_local(Vector3 point) const;

    void apply(const Tool &t, const Action &a, const int max_depth = -1, bool multithreaded = (JobPool::get_number_of_cores() > 1), unsigned char thread_count = std::max(JobPool::get_number_of_cores(), 2));

    const Point &get_corner(int index) const {return *corners[index % 8];}
    Point &get_corner(int index) {return *corners[index % 8];}
    Vector3 get_corner_pos(int index) const {return position + (GRID_VERTICES[index] / br_real_t(1ULL << subdiv_level));}

    int get_index_containing_pos(const Vector3 pos) const;

    unsigned char get_depth() {return subdiv_level;}
    Cell *get_child(int index) {return (has_children() ? &children[index] : NULL);}

    AABB get_aabb() const;

    std::unique_ptr<std::string> serialize() const;
    static std::unique_ptr<Cell> deserialize(const void *buf, size_t length);
};

}}

#endif