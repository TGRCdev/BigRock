#include "mesh.hpp"
#include <ostream>
#include "../defines.hpp"

namespace bigrock {
namespace mesh {

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 tex_coords)
{
    this->position = position;
    this->normal = normal;
    this->tex_coords = tex_coords;
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal)
{
    this->position = position;
    this->normal = normal;
    this->tex_coords = glm::vec2(0);
}

Vertex::Vertex(glm::vec3 position)
{
    this->position = position;
    this->normal = glm::vec3(0);
    this->tex_coords = glm::vec2(0);
}

Vertex::Vertex()
{
    this->position = glm::vec3(0);
    this->normal = glm::vec3(0);
    this->tex_coords = glm::vec2(0);
}

std::ostream &operator<<(std::ostream &os, const Mesh &mesh)
{
    for(auto v = mesh.vertices->begin(); v != mesh.vertices->end(); v++)
    {
        os << "v " << v->position.x << " " << v->position.y << " " << v->position.z << '\n';
    }

    for(auto surf = mesh.surfaces->begin(); surf != mesh.surfaces->end(); surf++)
    {
        auto index = surf->begin();
        while(index != surf->end())
        {
            os << "f " << ((*index) + 1);
            index++;
            os << " " << ((*index) + 1);
            index++;
            os << " " << ((*index) + 1);
            index++; 
            os << '\n';
        }
    }

    return os;
}

}}