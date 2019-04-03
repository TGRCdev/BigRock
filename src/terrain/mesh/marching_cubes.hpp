#ifndef BIGROCK_MARCHING_CUBES_HPP
#define BIGROCK_MARCHING_CUBES_HPP

#include "polygoniser.hpp"

namespace bigrock
{
    namespace terrain
    {
        class MarchingCubes : public Polygoniser
        {
            void polygonise_cell(const Cell &cell, unsigned int max_depth, const math::Transform &t, std::vector<Vertex> &temp_vertices) const;

            public:
            Mesh polygonise(const Cell &cell, unsigned int max_depth, const math::Transform &t = math::Transform()) const;
        };
    }
}

#endif