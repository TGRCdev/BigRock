#include "mesh.hpp"

namespace bigrock {
namespace mesh {

Mesh::Mesh()
{
    this->vertices = std::unique_ptr< std::vector<Vertex> >();
    this->surfaces = std::unique_ptr< std::vector<std::unique_ptr<std::vector<unsigned int> > > >();
}

}}