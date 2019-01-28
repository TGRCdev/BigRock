#ifndef BIGROCK_TERRAIN_H
#define BIGROCK_TERRAIN_H

namespace bigrock
{
    /*
        Responsibilities of Terrain:
        * Abstract away CSG operations, mesh generation
          and data structure
    */
    class Terrain
    {
        enum TERRAIN_METHOD
        {
            Surface_Nets,
        };

        static Terrain *create_terrain(TERRAIN_METHOD terrain_type); // Factory function
    };

    class SurfaceNetsTerrain : public Terrain
    {

    };
}

#endif
