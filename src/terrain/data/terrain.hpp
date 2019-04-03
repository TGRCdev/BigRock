#ifndef BIGROCK_TERRAIN_H
#define BIGROCK_TERRAIN_H

#include <boost/unordered_map.hpp>
#include <boost/container_hash/hash.hpp>

#include "../../math/vector3.hpp"
#include "../../math/transform.hpp"
#include "../../defines.hpp"

namespace bigrock
{
    namespace terrain
    {
        struct Vertex
        {
            Vertex(math::Vector3 location = math::Vector3::zero, br_real value = 0.0, math::Vector3 gradient = math::Vector3::zero, int material = -1)
            {
                this->location = location;
                this->value = value;
                this->gradient = gradient;
                this->material = material;
            }

            math::Vector3 location;
            br_real value;
            math::Vector3 gradient;
            int material;

            Vertex interpolate(const Vertex &other, br_real t) const;
            bool can_collapse(const Vertex &other) const;

            bool operator==(const Vertex &other) const
            {
                return (this->location == other.location) && (this->value == other.value) && (this->gradient == other.gradient) && (this->material == other.material);
            }
        };

        std::size_t hash_value(const Vertex &v);

        struct Tool;
        struct Action;

        class Cell;

        // Struct containing edges that intersect cube, and interpolated vertices on said edges
        struct SurfaceElement
        {
            SurfaceElement(const Cell &cell); // Setup edge config and vertices

            /* 
                12-bit bitflag for cube edges that intersect the isosurface
                For reference:
                EDGE (BIT) = [VERTEX] -> [VERTEX]
                0  (1)    = [0] -> [1]
                1  (2)    = [0] -> [2]
                2  (4)    = [1] -> [3]
                3  (8)    = [2] -> [3]
                4  (16)   = [4] -> [5]
                5  (32)   = [4] -> [6]
                6  (64)   = [5] -> [7]
                7  (128)  = [6] -> [7]
                8  (256)  = [0] -> [4]
                9  (512)  = [1] -> [5]
                10 (1024) = [2] -> [6]
                11 (2048) = [3] -> [7]
            */
            int edgeconfig;
            int vertconfig;
            Vertex edge_vertices[12]; // Vertices interpolated along intersected edges

            bool edge_intersects(int edge_index) {return (edgeconfig & (1 << edge_index)) != 0;}

            // Gets the average normal of the intersected edges
            math::Vector3 get_average_normal();

            /// The discrepancy is the square distance from the average normal to each edge vertex's normal
            /// A high discrepancy represents an irregular surface that isn't accurately described by the current
            /// subdivision level.
            br_real estimate_discrepancy();
        };

        class Cell
        {
            /// Child constructor
            /// Vertices must be set by the parent
            Cell(int subdiv_level);
            unsigned int subdiv_level;
            Vertex *vertices[8];
            int owned_vertices; // bitflag for vertices that this cell is allowed to delete
            Cell *children;

            static const float acceptableDiscrepancy = 0.1;

            public:
            /// Called when a sampled position is outside of the Cell.
            /// The edge function is inherited by children
            Vertex (*edge_function)(const Cell *cell, const math::Vector3 &rel_pos);
            static Vertex default_edge_function(const Cell *cell, const math::Vector3 &rel_pos);

            Cell(); // Empty constructor
            ~Cell();

            Vertex &get_vertex(int index) {return *vertices[index];}
            const Vertex &get_vertex(int index) const {return *vertices[index];}
            int get_owned_vertices() const {return owned_vertices;}
            bool owns_vertex(int index) const {return (owned_vertices & (1 << index)) != 0;}
            bool is_leaf() const {return children == NULL;}
            Cell *get_child(int index) {return &children[index];}
            const Cell *get_child(int index) const {return &children[index];}
            int get_index_containing_point(const math::Vector3 &point) const;
            unsigned int get_subdiv_level() const {return subdiv_level;}
            bool can_collapse() const;

            SurfaceElement get_surface_element() const;

            void apply_tool(const Tool &t, const Action &a);

            void subdivide();
            void undivide();

            /// Recursively optimize this cell
            void try_undivide();
            
            /// See if this cell requires additional detail
            void check_subdivide(const Tool &t);

            Vertex sample(const math::Vector3 &pos) const;

            friend class Terrain;
        };

        // TODO: Continue here

        class Terrain
        {
            Vertex edge_function(const Cell *cell, const math::Vector3 &relpos);

            public:
            Terrain();

            Cell &get_chunk(const math::Vector3 &chunkpos);
            
            boost::unordered_map<math::Vector3, Cell> chunks;
            boost::unordered_map<const Cell*, math::Vector3> chunk_positions;

            void apply_tool(const Tool &t, const Action &a);

            Vertex sample(const math::Vector3 &point) const;
        };
    }
}

#endif
