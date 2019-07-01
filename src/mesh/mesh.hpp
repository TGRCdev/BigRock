#ifndef BIGROCK_MESH_H
#define BIGROCK_MESH_H
#pragma once

#include <memory>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <map>

namespace bigrock {
namespace mesh {

struct Vertex
{
    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 tex_coords);
    Vertex(glm::vec3 position, glm::vec3 normal);
    explicit Vertex(glm::vec3 position);
    Vertex();

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

struct Mesh
{
    std::unique_ptr< std::vector<Vertex> > vertices;
    std::unique_ptr< std::vector< std::vector<unsigned int> > > surfaces;

    #ifndef NDEBUG
    // Only here for debug purposes
    // Outputs the mesh in OBJ format
    friend std::ostream &operator<<(std::ostream &os, const Mesh &mesh);
    #endif
};

}}

#endif