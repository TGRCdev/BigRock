#ifndef BIGROCK_TERRAIN_H
#define BIGROCK_TERRAIN_H

#include "chunk_mesh.hpp"
#include "../math/shape.hpp"
#include "../math/ray.hpp"
#include "../math/vector3i.hpp"

#include <map>
#include <vector>

namespace bigrock
{
    namespace terrain
    {
        class ShapeQueryResult
        {
            std::vector<int> materials;
            std::vector<float> volumes;

            public:
            int get_material_count() {return materials.size();}
            int get_material(const int index) {return materials[index];}
            float get_volume(const int index) {return volumes[index];}
        };

        struct RayQueryResult
        {
            bool hit;
            math::Vector3 point;
            math::Vector3 normal;
            int material;
        };

        struct PointQueryResult
        {
            float isovalue;
            int material;
        };

        class Terrain
        {
            protected:
            Terrain(const unsigned int chunk_size_v) : chunk_size(chunk_size_v) {}
            std::map<math::Vector3i, ChunkMesh> chunkmeshes;

            public:
            math::Vector3i get_chunk_pos_containing_point(const math::Vector3 &point) const;
            ChunkMesh *get_chunk_mesh_containing_point(const math::Vector3 &point);

            const unsigned int chunk_size;

            // Applies the material using the shape in the terrain.
            virtual void set_shape_data(const math::Shape &shape, const int material) = 0;

            // Should be used for terrain generation only. May be removed.
            virtual void set_point_data(const math::Vector3 &point, const int material) = 0;

            virtual PointQueryResult query_point(const math::Vector3 &point) const = 0;
            virtual RayQueryResult query_ray(const math::Ray &ray) const = 0;
            virtual ShapeQueryResult query_shape(const math::Shape &shape) const = 0;

            virtual void update_chunks() = 0;
            ChunkMesh *get_chunk_mesh(const math::Vector3i position);

            virtual char *serialize() = 0;
            static Terrain *deserialize(const char* data, const unsigned long data_size); // TODO

            enum TERRAIN_METHOD
            {
                MARCHING_CUBES_GRID
            };

            static Terrain *create_terrain(const TERRAIN_METHOD method, unsigned int chunk_size);
        };
    }
}

#endif