#ifndef BIGROCK_TERRAIN_MESH_H
#define BIGROCK_TERRAIN_MESH_H

#include "vector3.h"
#include <vector>
#include <map>

namespace bigrock
{
    class Terrain;

    struct Triangle
    {
        Vector3 vertices[3];
        Vector3 normals[3];
    };

    struct Surface
    {
        std::vector<Triangle*> triangles;
    };

    class TerrainMesh
    {
        Terrain *data;
        std::vector<Surface> surfaces;

        public:
        Terrain *get_terrain_data() const {return data;}
        const std::vector<Surface> *get_surfaces() const {return &surfaces;}

        friend class Terrain;
    };
}

#endif