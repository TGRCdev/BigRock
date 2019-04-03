#ifndef BIGROCK_POLYGONISER_H
#define BIGROCK_POLYGONISER_H

#include "../../math/vector3.hpp"
#include "../../math/transform.hpp"

#include "../data/terrain.hpp"

#include <vector>
#include <map>

namespace bigrock
{
    namespace terrain
    {
        class Mesh
        {
            public:
            struct Vertex
            {
                Vertex(const bigrock::terrain::Vertex &v)
                {
                    this->position = v.location;
                    this->normal = v.gradient;
                }

                math::Vector3 position;
                math::Vector3 normal;
                float uv_x, uv_y; // TODO: Vector2
            };

            std::vector<Vertex> vertices;
            std::map<unsigned int, std::vector<unsigned int> > material_indices;
        };

        class Polygoniser
        {
            public:
            virtual Mesh polygonise(const Cell &cell, unsigned int max_depth, const math::Transform &t = math::Transform()) const = 0;
        };
    }
}

#endif