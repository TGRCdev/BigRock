#include "terrain.hpp"
#include "marching_cubes_grid.hpp"

namespace bigrock
{
    namespace terrain
    {
        ChunkMesh *Terrain::get_chunk_mesh(const math::Vector3i position)
        {
            std::map<math::Vector3i, ChunkMesh>::iterator it = chunkmeshes.find(position);
            if(it == chunkmeshes.end())
                return NULL;
            else
                return &it->second;
        }

        math::Vector3i Terrain::get_chunk_pos_containing_point(const math::Vector3 &point) const
        {
            math::Vector3 vec = point / chunk_size;
            vec.floor();
            return vec;
        }

        ChunkMesh *Terrain::get_chunk_mesh_containing_point(const math::Vector3 &point)
        {
            return get_chunk_mesh(get_chunk_pos_containing_point(point));
        }

        Terrain *Terrain::create_terrain(const TERRAIN_METHOD method, unsigned int chunk_size)
        {
            switch(method)
            {
                case MARCHING_CUBES_GRID:
                    return new terrain::MarchingCubesGrid(chunk_size);
                default:
                    return NULL;
            }
        }
    }
}