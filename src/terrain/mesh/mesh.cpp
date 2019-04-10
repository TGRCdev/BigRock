#include "mesh.hpp"

namespace bigrock {
namespace terrain {
namespace mesh {

void Mesh::add_surface(const unsigned int &surface, const Vertex *input, const unsigned int &vert_count)
{
    // Based on example code by moliszmaki on Stack Overflow
    // https://stackoverflow.com/questions/14396788/how-can-i-generate-indices-from-vertex-list-in-linear-time/14417234#14417234

    unsigned int current_index = vbuffer.size();
    int before_size = surfaces.size();
    std::vector<unsigned int> *indices = &surfaces[surface];
    if(before_size != surfaces.size())
    {
        si_buffer.resize(surface_indices.size());
        std::copy(surface_indices.begin(), surface_indices.end(), si_buffer.begin());
    }

    for(unsigned int i = 0; i < vert_count; i++)
    {
        std::pair<Vertex, unsigned int> current_pair(input[i], current_index);
        std::set<std::pair<Vertex, unsigned int> >::iterator it = vertices.find(current_pair);
        if(it != vertices.end())
            indices->push_back(it->second);
        else
        {
            vertices.insert(current_pair);
            indices->push_back(current_index);
            current_index++;
        }
    }

    vbuffer.resize(vertices.size());
    for(std::set<std::pair<Vertex, unsigned int>>::iterator it = vertices.begin(); it != vertices.end(); it++)
        vbuffer[it->second] = it->first;
}

void Mesh::clear_surface(const unsigned int &surface)
{
    surfaces.erase(surface);
    if(surface_indices.erase(surface) > 0);
    {
        si_buffer.resize(surface_indices.size());
        std::copy(surface_indices.begin(), surface_indices.end(), si_buffer.begin());
    }
}

const Vertex *Mesh::get_vertex_array() const
{
    return vbuffer.data();
}

unsigned int Mesh::get_vertex_count() const
{
    return vbuffer.size();
}

const unsigned int *Mesh::get_surface_index_array(const unsigned int &surface) const
{
    std::map<unsigned int, std::vector<unsigned int> >::const_iterator it = surfaces.find(surface);
    if(it == surfaces.end())
        return NULL;
    else
        return it->second.data();
}

unsigned int Mesh::get_surface_index_count(const unsigned int &surface) const
{
    std::map<unsigned int, std::vector<unsigned int> >::const_iterator it = surfaces.find(surface);
    if(it == surfaces.end())
        return 0;
    else
        return it->second.size();
}

const unsigned int *Mesh::get_surface_indices() const
{
    return si_buffer.data();
}

unsigned int Mesh::get_surface_count() const
{
    return si_buffer.size();
}

}}}