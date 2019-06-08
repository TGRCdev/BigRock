#ifndef BIGROCK_MARCHING_CUBES_CPU
#define BIGROCK_MARCHING_CUBES_CPU
#pragma once

#include "terrainmesher.hpp"
#include "../data/jobpool.hpp"

namespace bigrock {
namespace mesh {

// Marching cubes algorithm on the CPU
class MarchingCubesCPU : public TerrainMesher
{
    void generate_unthreaded(const Cell &cell, const unsigned char max_depth, std::map<unsigned char, std::vector<glm::vec3>> &surfaces) const;

    public:
    bool multithreaded = false;
    unsigned char thread_count = JobPool::get_number_of_cores();

    std::unique_ptr<Mesh> generate(const Cell &cell, const unsigned char max_depth = BR_MAX_CELL_DEPTH) const;
};

}}

#endif