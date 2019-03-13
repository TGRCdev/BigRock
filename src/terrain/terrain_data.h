#ifndef BIGROCK_TERRAIN_DATA_H
#define BIGROCK_TERRAIN_DATA_H

#include "../defines.hpp"
#include "../math/vector3.hpp"
#include "tool.h"

namespace bigrock
{
    namespace terrain
    {
        struct Vertex
        {
            br_real value;
            const math::Vector3 position;  
            int material;

            // Gets the vertex between this vertex and the other vertex at t
            Vertex interpolate(const Vertex &other, br_real t);

            // Returns true if both Vertices are similar enough to be collapsed into one vertex
            bool can_collapse(const Vertex &other);
        };

        class Cell
        {
            const unsigned int subdiv_count;
            Vertex *vertices[8];
            Cell *children;
            unsigned int owned_vertices;

            public:

            // Returns true if this Cell has no children
            bool is_leaf() const;
            
            // Divides the Cell into 8 child cells and shares its vertices
            void subdivide();

            // Deletes the Cell's children and their owned vertices
            void undivide();

            // Checks if all child values are similar. If so, undivide.
            void try_undivide();

            // Checks if 
            void try_subdivide(const Tool &t);

            int get_child(const int index);

            // Gets the child index that contains the given point
            int get_index_containing_point(const math::Vector3 &point) const;
        };
    }
}

#endif