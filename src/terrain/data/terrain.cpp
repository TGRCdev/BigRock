#include "terrain.hpp"
#include "grid_vertices.hpp"
#include "tool.hpp"
#include "action.hpp"

#include "../../math/math_funcs.hpp"

#include <boost/unordered_set.hpp>

namespace bigrock
{
    namespace terrain
    {
        #pragma region Vertex
        Vertex Vertex::interpolate(const Vertex &other, br_real t) const
        {
            if(t < 0)
                return *this;
            else if(t > 1)
                return other;
            else
            {
                Vertex ret;
                ret.value = math::lerp(this->value, other.value, t);
                ret.location = this->location.lerp(other.location, t);
                ret.gradient = this->gradient.lerp(other.gradient, t).normalized();
                if(this->material == other.material)
                    ret.material = this->material;
                else
                {
                    float val = math::lerp(this->value, -other.value, t);
                    if(val < 0)
                        ret.material = other.value;
                    else
                        ret.material = this->value;
                }
                    
                return ret;
            }
        }

        bool Vertex::can_collapse(const Vertex &other) const
        {
            return ((this->value < 1.5 && other.value < 1.5) || ((this->material == other.material) && this->value >= 1.5 && other.value >= 1.5));
        }

        std::size_t hash_value(const Vertex &v)
        {
            std::size_t h1 = math::hash_value(v.location);
            std::size_t h2 = boost::hash_value<br_real>(v.value);
            std::size_t h3 = math::hash_value(v.gradient);
            std::size_t h4 = boost::hash_value<int>(v.material);

            return h1 ^ h2 ^ h3 ^ h4;
        }
        #pragma endregion

        #pragma region Cell

        Cell::Cell()
        {
            children = NULL;
            owned_vertices = 0xFF;
            subdiv_level = 0;
            edge_function = default_edge_function;
            for(int i = 0; i < 8; i++)
                vertices[i] = new Vertex(GRID_VERTICES[i]);
        }

        Cell::Cell(int subdiv_level)
        {
            children = NULL;
            this->owned_vertices = 0;
            this->subdiv_level = subdiv_level;
            edge_function = default_edge_function;
            // Vertices are setup by the parent
        }

        Cell::~Cell()
        {
            if(!is_leaf())
                for(int i = 0; i < 8; i++)
                {
                    children[i].~Cell();
                }
                free(children);
            
            for(int i = 0; i < 8; i++)
                if (owns_vertex(i))
                    delete vertices[i];
        }

        Vertex Cell::default_edge_function(const Cell *cell, const math::Vector3 &rel_pos)
        {
            return Vertex();
        }

        bool Cell::can_collapse() const
        {
            if(!is_leaf())
                return false;

            bool collapse = true;
            for(int i = 1; i < 8 && collapse; i++)
                collapse = collapse && vertices[i-1]->can_collapse(*vertices[i]);
            return collapse;
        }

        SurfaceElement Cell::get_surface_element() const
        {
            return SurfaceElement(*this);
        }

        void Cell::apply_tool(const Tool &t, const Action &a)
        {
            for(int i = 0; i < 8; i++)
                if((owned_vertices & (1 << i)) != 0)
                    a.update(t, *vertices[i]);

            if(is_leaf())
                check_subdivide(t);

            if(!is_leaf())
                for(int i = 0; i < 8; i++)
                    children[i].apply_tool(t, a);
        }

        void Cell::subdivide()
        {
            if(children != NULL)
                return;
            
            // Step 1. Index all relevant vertices

            Vertex *child_vertices[3][3][3];
            
            // Emplace all existing vertices
            child_vertices[0][0][0] = vertices[0];
            child_vertices[2][0][0] = vertices[1];
            child_vertices[0][2][0] = vertices[2];
            child_vertices[2][2][0] = vertices[3];
            child_vertices[0][0][2] = vertices[4];
            child_vertices[2][0][2] = vertices[5];
            child_vertices[0][2][2] = vertices[6];
            child_vertices[2][2][2] = vertices[7];

            // Begin generating new vertices
            for(int x = 0; x < 3; x++)
                for(int y = 0; y < 3; y++)
                    for(int z = 0; z < 3; z++)
                        if (x != 1 && y != 1 && z != 1)
                            continue;
                        else
                            child_vertices[x][y][z] = new Vertex(sample(math::Vector3(x, y, z)));
            
            // Step 2. Pass vertices and assign ownership
            children = (Cell*)malloc(sizeof(Cell) * 8);

            for(int i = 0; i < 8; i++)
                children[i] = Cell(subdiv_level + 1);

            boost::unordered_set<math::Vector3> owned;
            for(int i = 0; i < 8; i++)
            {
                owned.insert(GRID_VERTICES[i] * 2);
                math::Vector3 cellpos = GRID_VERTICES[i];
                for(int j = 0; j < 8; j++)
                {
                    math::Vector3 vpos = GRID_VERTICES[j];
                    children[i].vertices[j] = child_vertices[(int)(vpos.x + cellpos.x)][(int)(vpos.y + cellpos.y)][(int)(vpos.z + cellpos.z)];
                    if(owned.find(vpos + cellpos) == owned.end())
                    {
                        owned.insert(vpos + cellpos);
                        children[i].owned_vertices |= (1 << i);
                    }
                }
                children[i].owned_vertices = children[i].owned_vertices ^ 0xFF;
            }
        }

        void Cell::undivide()
        {
            if(children == NULL)
                return;
            
            for(int i = 0; i < 8; i++)
                children[i].~Cell();
            free(children);
            children = NULL;
        }

        void Cell::try_undivide()
        {
            if (is_leaf())
                return;

            bool collapse = true;
            for(int i = 0; i < 7 && collapse; i++)
            {
                children[i].try_undivide();
                
                collapse = collapse && children[i].can_collapse();
                collapse = collapse && children[i].vertices[7]->can_collapse(*children[i+1].vertices[0]);
            }

            if(collapse)
                undivide();
        }

        void Cell::check_subdivide(const Tool &t)
        {
            if(subdiv_level < t.get_min_subdiv())
                subdivide();
            else if(subdiv_level < t.get_max_subdiv())
                if(get_surface_element().estimate_discrepancy() > acceptableDiscrepancy)
                    subdivide();
        }

        int Cell::get_index_containing_point(const math::Vector3 &pos) const
        {
            int index = 0;
            math::Vector3 midpoint = vertices[0]->location.lerp(vertices[7]->location, 0.5);
            if(pos.x > midpoint.x)
                index |= 1;
            if(pos.y > midpoint.y)
                index |= 2;
            if(pos.z > midpoint.z)
                index |= 4;
            return index;
        }

        Vertex Cell::sample(const math::Vector3 &pos) const
        {
            if(is_leaf())
            {
                math::Vector3 t = (pos - vertices[0]->location) / (vertices[7]->location - vertices[0]->location);

                Vertex x_plane[4];
                x_plane[0] = vertices[0]->interpolate(*vertices[1], t.x);
                x_plane[1] = vertices[2]->interpolate(*vertices[3], t.x);
                x_plane[2] = vertices[4]->interpolate(*vertices[5], t.x);
                x_plane[3] = vertices[6]->interpolate(*vertices[7], t.x);

                Vertex y_line[2];
                y_line[0] = x_plane[0].interpolate(x_plane[1], t.y);
                y_line[1] = x_plane[2].interpolate(x_plane[3], t.y);

                return y_line[0].interpolate(y_line[1], t.z);
            }
            else
            {
                return children[get_index_containing_point(pos)].sample(pos);
            }
        }

        #pragma endregion

        #pragma region SurfaceElement

        Vertex vertex_interp(const Vertex &vert1, const Vertex &vert2)
        {
            if(fabs(1.5-vert1.value) < 0.00001)
                return (vert1);
            if(fabs(1.5-vert2.value) < 0.00001)
                return (vert2);
            if(fabs(vert1.value - vert2.value) < 0.0001)
                return (vert1);
            
            double t = (1.5 - vert1.value) / (vert2.value - vert1.value);
            return vert1.interpolate(vert2, t);
        }

        #define sign_change(a, b) (a.value - 1.5) * (b.value - 1.5) <= 0

        SurfaceElement::SurfaceElement(const Cell &cell)
        {
            edgeconfig = 0;
            vertconfig = 0;

            // Step 1. Setup edge configuration
            if (sign_change(cell.get_vertex(0), cell.get_vertex(1)))
                edgeconfig |= 1;
            if (sign_change(cell.get_vertex(0), cell.get_vertex(2)))
                edgeconfig |= 2;
            if (sign_change(cell.get_vertex(1), cell.get_vertex(3)))
                edgeconfig |= 4;
            if (sign_change(cell.get_vertex(2), cell.get_vertex(3)))
                edgeconfig |= 8;
            if (sign_change(cell.get_vertex(4), cell.get_vertex(5)))
                edgeconfig |= 16;
            if (sign_change(cell.get_vertex(4), cell.get_vertex(6)))
                edgeconfig |= 32;
            if (sign_change(cell.get_vertex(5), cell.get_vertex(7)))
                edgeconfig |= 64;
            if (sign_change(cell.get_vertex(6), cell.get_vertex(7)))
                edgeconfig |= 128;
            if (sign_change(cell.get_vertex(0), cell.get_vertex(4)))
                edgeconfig |= 256;
            if (sign_change(cell.get_vertex(1), cell.get_vertex(5)))
                edgeconfig |= 512;
            if (sign_change(cell.get_vertex(2), cell.get_vertex(6)))
                edgeconfig |= 1024;
            if (sign_change(cell.get_vertex(3), cell.get_vertex(7)))
                edgeconfig |= 2048;
            
            // Step 2: Setup vert configuration
            if(cell.get_vertex(0).value > 1.5)
                vertconfig |= 1;
            if(cell.get_vertex(1).value > 1.5)
                vertconfig |= 2;
            if(cell.get_vertex(2).value > 1.5)
                vertconfig |= 4;
            if(cell.get_vertex(3).value > 1.5)
                vertconfig |= 8;
            if(cell.get_vertex(4).value > 1.5)
                vertconfig |= 16;
            if(cell.get_vertex(5).value > 1.5)
                vertconfig |= 32;
            if(cell.get_vertex(6).value > 1.5)
                vertconfig |= 64;
            if(cell.get_vertex(7).value > 1.5)
                vertconfig |= 128;
            
            // Step 2. Interpolate edge intersections
            if ((edgeconfig & 1) != 0)
                edge_vertices[0] = vertex_interp(cell.get_vertex(0), cell.get_vertex(1));
            if ((edgeconfig & 2) != 0)
                edge_vertices[1] = vertex_interp(cell.get_vertex(0), cell.get_vertex(2));
            if ((edgeconfig & 4) != 0)
                edge_vertices[2] = vertex_interp(cell.get_vertex(1), cell.get_vertex(3));
            if ((edgeconfig & 8) != 0)
                edge_vertices[3] = vertex_interp(cell.get_vertex(2), cell.get_vertex(3));
            if ((edgeconfig & 16) != 0)
                edge_vertices[4] = vertex_interp(cell.get_vertex(4), cell.get_vertex(5));
            if ((edgeconfig & 32) != 0)
                edge_vertices[5] = vertex_interp(cell.get_vertex(4), cell.get_vertex(6));
            if ((edgeconfig & 64) != 0)
                edge_vertices[6] = vertex_interp(cell.get_vertex(5), cell.get_vertex(7));
            if ((edgeconfig & 128) != 0)
                edge_vertices[7] = vertex_interp(cell.get_vertex(6), cell.get_vertex(7));
            if ((edgeconfig & 256) != 0)
                edge_vertices[8] = vertex_interp(cell.get_vertex(0), cell.get_vertex(4));
            if ((edgeconfig & 512) != 0)
                edge_vertices[9] = vertex_interp(cell.get_vertex(1), cell.get_vertex(5));
            if ((edgeconfig & 1024) != 0)
                edge_vertices[10] = vertex_interp(cell.get_vertex(2), cell.get_vertex(6));
            if ((edgeconfig & 2048) != 0)
                edge_vertices[11] = vertex_interp(cell.get_vertex(3), cell.get_vertex(7));
        }

        math::Vector3 SurfaceElement::get_average_normal()
        {
            math::Vector3 norm;
            int verts = 0;

            for(int i = 0; i < 12; i++)
            {
                if(edge_intersects(i))
                {
                    norm += edge_vertices[i].gradient;
                    verts += 1;
                }
            }

            return (norm / verts).normalized();
        }

        br_real SurfaceElement::estimate_discrepancy()
        {
            math::Vector3 avg_normal = get_average_normal();
            br_real sqr_dist = 0.0;

            for(int i = 0; i < 12; i++)
            {
                if(edge_intersects(i))
                    sqr_dist += avg_normal.distance_squared_to(edge_vertices[i].gradient);
            }

            return sqr_dist;
        }

        #pragma endregion

        #pragma region Terrain

        Vertex Terrain::edge_function(const Cell *cell, const math::Vector3 &rel_pos)
        {
            math::Vector3 chunk = chunk_positions[cell];
            math::Vector3 actual_chunk = chunk + rel_pos;
            return sample(actual_chunk);
        }

        Cell &Terrain::get_chunk(const math::Vector3 &chunkpos)
        {
            return chunks[chunkpos];
        }

        void Terrain::apply_tool(const Tool &t, const Action &a)
        {
            math::AABB tool_aabb = t.get_aabb();
            math::Vector3 min = tool_aabb.origin.floor();
            math::Vector3 max = (tool_aabb.origin + tool_aabb.size).ceil();
            for(int x = min.x; x < max.x; x++)
                for(int y = min.y; y < max.y; y++)
                    for(int z = min.z; z < max.z; z++)
                        get_chunk(math::Vector3(x, y, z)).apply_tool(t, a);
        }

        Vertex Terrain::sample(const math::Vector3 &local_point) const
        {
            math::Vector3 chunk_pos = local_point.floor();
            math::Vector3 rel_pos = local_point - chunk_pos;
            boost::unordered_map<math::Vector3, Cell>::const_iterator it = chunks.find(chunk_pos);
            if(it == chunks.end())
                return Vertex();
            else
                return it->second.sample(rel_pos);
        }

        #pragma endregion
    }
}
