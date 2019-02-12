#ifndef BIGROCK_MARCHING_CUBES_H
#define BIGROCK_MARCHING_CUBES_H

#include "terrain.hpp"
#include "../math/vector3i.hpp"

#include <map>
#include <set>

namespace bigrock
{
    namespace terrain
    {
        class MarchingCubesGrid : public Terrain
        {
            class Chunk
            {
                public:
                struct Voxel
                {
                    Voxel() {isovalue = -1;material_index = -1;}
                    Voxel(float isovalue, int material_index)
                    {
                        this->isovalue = isovalue;
                        this->material_index = (this->isovalue < 0 ? -1 : material_index);
                    }

                    float isovalue;
                    int material_index;

                    /*
                        Compute the union between two voxels
                        Other voxel gets precedence
                    */
                    Voxel merge(const Voxel &other) const
                    {
                        if(this->isovalue == -1)
                            return other;
                        else if(other.isovalue == -1)
                            return *this;
                        
                        if(other.isovalue >= 0)
                            return other;
                        else // -1 < other.isovalue < 0
                            return *this; //TODO: Test this, change if possible
                    }

                    Voxel interp(const Voxel &other, float t) const
                    { // TODO: Clean this garbage
                        if(t <= 0)
                            return *this;
                        else if(t >= 1)
                            return other;
                        else
                        {
                            if(this->isovalue > 0 && other.isovalue > 0)
                            {
                                if(this->material_index == other.material_index)
                                {
                                    float iso = this->isovalue + ((other.isovalue - this->isovalue) * t);
                                    return Voxel(iso, this->material_index);
                                }
                                else
                                {
                                    float iso = this->isovalue + ((-other.isovalue - this->isovalue) * t);
                                    if(iso < 0)
                                        return Voxel(-iso, other.material_index);
                                    else
                                        return Voxel(iso, this->material_index);
                                }
                            }
                            else
                            {
                                float iso = this->isovalue + ((other.isovalue - this->isovalue) * t);
                                if(this->isovalue > 0)
                                    return Voxel(iso, (iso > 0 ? this->material_index : -1));
                                else
                                    return Voxel(iso, (iso > 0 ? other.material_index : -1));
                            }
                        }
                    }
                };

                private:
                std::map<math::Vector3i, Voxel> voxels;

                public:
                Voxel get_voxel(const math::Vector3i &position) const;
                void set_voxel(const math::Vector3i &position, Voxel voxel);
            };
            std::map<math::Vector3i, Chunk> chunks;
            std::set<math::Vector3i> updates;

            // Empty constructor
            MarchingCubesGrid(unsigned int chunk_size_v) : Terrain(chunk_size_v) {}; 

            // Deserialize constructor
            MarchingCubesGrid(const char *data, const unsigned long data_size);

            // Get the chunk at this chunk position
            Chunk *get_chunk(const math::Vector3i &point);
            const Chunk *get_chunk(const math::Vector3i &point) const;

            Chunk::Voxel get_voxel(const math::Vector3i &point) const;
            void set_voxel(const math::Vector3i &point, Chunk::Voxel voxel);

            static math::Vector3 vertex_interp(math::Vector3 p1, math::Vector3 p2, float val1, float val2);
            void polygonise(const math::Vector3i &leftcorner, ChunkMesh &mesh);

            public:
            void set_shape_data(const math::Shape &shape, const int material);
            void set_point_data(const math::Vector3 &point, const int material);

            PointQueryResult query_point(const math::Vector3 &point) const;
            RayQueryResult query_ray(const math::Ray &ray) const;
            ShapeQueryResult query_shape(const math::Shape &shape) const;

            void update_chunks();

            char *serialize() const;

            friend class Terrain;
        };
    }
}

#endif