#ifndef BIGROCK_MESH_H
#define BIGROCK_MESH_H

#include "vertex.hpp"

#include <vector>
#include <set>
#include <unordered_set>
#include <map>

namespace bigrock {
namespace terrain {
namespace mesh {

/*
 * Note: This class is for constructing dynamic meshes, and does not even
 * attempt to optimize vertices.
 */
class Mesh
{
    struct verthash
    {
        std::size_t operator()(const std::pair<Vertex, unsigned int> &key) const
        {
            return ((std::hash<float>()(key.first.position.x)
               ^ (std::hash<float>()(key.first.position.y) << 1)) >> 1)
               ^ (std::hash<float>()(key.first.position.z) << 1);
        }
    };

    std::vector<Vertex> vbuffer;
    std::unordered_set<std::pair<Vertex, unsigned int>, verthash> vertices;
    std::map<unsigned int, std::vector<unsigned int> > surfaces;
    
    std::set<unsigned int> surface_indices;
    std::vector<unsigned int> si_buffer;

    public:
    void add_surface(const unsigned int &surface, const Vertex *vertices, const unsigned int &vert_count);
    void clear_surface(const unsigned int &surface);

    const Vertex *get_vertex_array() const;
    unsigned int get_vertex_count() const;

    // Returns an array of indices on a given surface material
    const unsigned int *get_surface_index_array(const unsigned int &surface) const;
    // Returns the number of indices on a given surface material
    unsigned int get_surface_index_count(const unsigned int &surface) const;

    // Returns an array of valid surface indices
    const unsigned int *get_surface_indices() const;
    // Returns the number of valid surface indices
    unsigned int get_surface_count() const;
};

}}}

#endif