#ifndef BIGROCK_CELL_H
#define BIGROCK_CELL_H

#include "../defines.hpp"

#include "point.hpp"
#include "shape.hpp"
#include "point_query.hpp"

#include <memory>

namespace bigrock {

    /// A chunk of terrain, represented in local coordinates between (0,0,0) and (1,1,1)
    /// Has pointers to 8 Points that represent the corners of the Cell.
    class Cell
    {
        std::shared_ptr<Point> corners[8];
        /// corners used to have a owned_corners bitflag variable for defining
        /// what corners could be deleted when this Cell was being collapsed.
        /// This assumption was only valid when Point corners were shared among
        /// child Cells, and not sibling cells.
        Cell *children;

        unsigned char depth;

        // Creates an empty Cell without filling in corners or children
        Cell(unsigned char depth) : depth(depth), children(nullptr) {}
    public:
        // Create a root Cell and its corners at depth 0
        Cell();

        // Returns the Point at corner index
        Point &get_corner(unsigned char index);
        // Returns the const Point at corner index
        const Point &get_corner(unsigned char index) const;

        // Returns the child at index, or nullptr if the Cell is a leaf
        Cell *get_child(unsigned char index);
        // Returns the child at index, or nullptr if the Cell is a leaf
        const Cell *get_child(unsigned char index) const;

        // Returns true if the Cell has no child Cells
        bool is_leaf() const {return children == nullptr;}
        // Returns true if the Cell has child Cells
        bool has_children() const {return children != nullptr;}

        /// Returns the index of the child containing the given point.
        /// point is a vector between (0,0,0) and (1,1,1)
        static unsigned char get_child_with_point(glm::vec3 point);

        /// Splits the Cell into 8 child Cells, interpolating the corners
        /// of the children.
        ///
        /// If the Cell already has children, does nothing.
        void subdivide();

        /// Collapses the Cell's children and deletes their data.
        ///
        /// Does nothing if the Cell has no children.
        void undivide();

        // Query Functions

        /// Traverse the Cell's children until a leaf is reached, then 
        /// interpolates between the leaf's corners and returns the 
        /// resulting data.
        ///
        /// When calling query_point on a Cell, point must have all
        /// components between 0 and 1. Otherwise, returns default
        /// data.
        PointQuery query_point(glm::vec3 point) const;
    };

}

#endif