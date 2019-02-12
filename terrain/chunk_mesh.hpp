#ifndef BIGROCK_TERRAIN_MESH_H
#define BIGROCK_TERRAIN_MESH_H

#include "../math/vector3.hpp"

#include <vector>
#include <map>

namespace bigrock
{
    namespace terrain
    {
        struct Vertex
        {
            Vertex() {}
            Vertex(math::Vector3 position, math::Vector3 normal = math::Vector3()) {this->position = position; this->normal = normal;}

            math::Vector3 position;
            math::Vector3 normal;
        };

        struct Surface
        {
            std::vector<int> indices;

            int get_index_count() const {return indices.size();}
            int get_index(int index) const {return indices[index];}
        };

        struct ChunkMesh
        { // TODO: Rethink this
            std::vector<Vertex> vertices;
            std::vector<Surface> surfaces;

            int get_vertex_count() const {return vertices.size();}
            int get_surface_count() const {return surfaces.size();}

            Vertex get_vertex(int index) const {return vertices[index];}
            void set_vertex(int index, Vertex vertex) {vertices[index] = vertex;}
            int add_vertex(Vertex vertex)
            {
                vertices.push_back(vertex);
                return vertices.size() - 1;
            }

            Surface get_surface(int index) const {return surfaces[index];}
            void set_surface(int index, Surface surface) {surfaces[index] = surface;}
            int add_surface(Surface surface)
            {
                surfaces.push_back(surface);
                return surfaces.size() - 1;
            }
        };
    }
}

#endif