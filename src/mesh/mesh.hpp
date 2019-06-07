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
    glm::vec3 point;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

struct Mesh
{
    Mesh();

    std::unique_ptr< std::vector<Vertex> > vertices;
    unsigned char surface_count;
    std::unique_ptr< std::vector< std::unique_ptr< std::vector<unsigned int> > > > surfaces; 
};

}}

#endif