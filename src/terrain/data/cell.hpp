#ifndef BIGROCK_CELL_H
#define BIGROCK_CELL_H

#include "cell_vertex.hpp"
#include "../grid_vertices.hpp"
#include "../../math/aabb.hpp"
#include "tool.hpp"
#include "action.hpp"

namespace bigrock {
namespace terrain {
namespace data {

class Cell
{
    Cell(const unsigned int &subdiv_level); // Child Constructor

    CellVertex *vertices[8];
    Cell *children;
    unsigned int subdiv_level;

    // Bitflag for vertices that this cell can delete
    int owned_vertices;

    public:
    Cell(); // Default Constructor
    ~Cell();
    // Getters

    CellVertex *get_vertex(const int &index) {return vertices[index];}
    const CellVertex *get_vertex(const int &index) const {return vertices[index];}
    Cell *get_child(const int &index) {return is_leaf() ? NULL : children + index;}
    const Cell *get_child(const int &index) const {return is_leaf() ? NULL : children + index;}
    unsigned int get_subdiv_level() const {return subdiv_level;}

    math::Vector3 calc_gradient(const math::Vector3 &point, const br_real &size = 1.0) const;

    // Returns true if this cell has no children
    bool is_leaf() const {return children == NULL;}
    // Returns true if this cell can be collapsed without losing unique data
    bool can_collapse() const;

    void subdivide();
    void undivide();

    // Checks if this cell children can be collapsed
    void try_undivide();

    // Interpolate within this cell and its children and return the result
    CellVertex sample(const math::Vector3 &pos) const;
    // Same as sample, but uses pos as (0,0,0) to (1,1,1) within the cell
    CellVertex sample_local(const math::Vector3 &pos) const;

    void apply_tool(const Tool &tool, const Action &action);

    int get_index_containing_pos(const math::Vector3 &pos) const;
    int get_index_containing_local_pos(const math::Vector3 &pos) const;

    math::AABB get_aabb() const;
};

}}}

#endif
