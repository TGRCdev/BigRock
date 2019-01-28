#ifndef BIGROCK_SURFACE_NETS_H
#define BIGROCK_SURFACE_NETS_H

#include "terrain.h"
#include "vector3.h"

#include <set>

namespace bigrock
{
    class SNOctreeRoot;

    struct SNOctreeData
    {
        float isoval;
        int material_index;

        bool can_collapse(const SNOctreeData &other);
        SNOctreeData interpolate(const SNOctreeData &other, const float t);
    };

    // Octree for surface nets
    class SNOctree
    {
        protected:
        static Vector3 get_grid_vertex(const int index)
        {
            Vector3 verts[8] = {
                Vector3(0,0,0),
                Vector3(1,0,0),
                Vector3(0,1,0),
                Vector3(1,1,0),
                Vector3(0,0,1),
                Vector3(1,0,1),
                Vector3(0,1,1),
                Vector3(1,1,1)
            };

            return verts[index];
        }

        SNOctree(SNOctree *parent, SNOctreeRoot *root, const unsigned int &depth, const Vector3 &relative_position, const Vector3 &relative_size, const SNOctreeData &data);
        SNOctree();

        // Performance variables (No need to serialize these)
        SNOctree *parent;
        SNOctreeRoot *root;

        Vector3 relative_position; // Position measured from (0,0,0) to (1,1,1)
        Vector3 relative_size; // Size measured from (0,0,0) to (1,1,1)

        unsigned int depth; // Distance down from root

        // Integral variables (Serialize these)
        bool is_leaf; // If true, data is type (SNOctreeData*). If false, data is type (SNOctree[8]*)
        void *data; // Either contains children or data

        public:
        ~SNOctree();

        // Methods
        SNOctreeData get_data() const; // Gets data if is_leaf, else gets data of lowest child leaf
        void set_data(const SNOctreeData &data); // Sets the data if is_leaf, else throws exception
        SNOctree *get_child(const int index); // Gets data[index] if is_leaf, else throws exception
        const SNOctree *get_child(const int index) const;

        SNOctree *get_leaf(); // Gets the lowest leaf that shares this Octree's position
        const SNOctree *get_leaf() const;
        bool is_leaf_node() const {return is_leaf;}
        bool is_root_node() const;
        
        int get_index_containing_point(const Vector3 &point) const;
        bool contains_point(const Vector3 &point) const;

        SNOctreeData get_interp_data(const Vector3 &point) const;
        SNOctree *get_closest_octant(const Vector3 &point);
        const SNOctree *get_closest_octant(const Vector3 &point) const;

        void subdivide(); // Expands this octant into 8 children if is_leaf, else does nothing
        void collapse(); // Combines this octant into a leaf if !is_leaf, else does nothing
        bool can_collapse(); // Checks if the children in this octant are similar enough to be collapsed. Return false if is_leaf

        void optimize(); // Tries to recursively collapse children

        unsigned int get_depth() const {return depth;} // Gets the downward distance from root

        Vector3 get_position() const; // Gets the position scaled by the Root's size
        Vector3 get_relative_position() const;// Gets the position independent of the Root's size

        Vector3 get_size() const; // Gets the size scaled by the Root's size
        Vector3 get_relative_size() const;// Gets the size independent of the Root's size
    };

    class SNOctreeRoot : public SNOctree
    {
        public:
        SNOctreeRoot(const Vector3 &size = Vector3(1,1,1));

        Vector3 size;
    };

    class SurfaceNetsTerrain : public Terrain
    {
        SNOctreeRoot octree;

        void create_mesh();
        bool mesh_needs_update;

        unsigned int polygonise(SNOctree *octant); // Polygonise this octant and return the lowest depth of children

        public:
        void update_mesh(bool force_refresh=false); // Apply terrain changes to the TerrainMesh. force_refresh forces every Octant to be repolygonized
        void set_shape_data(const Shape &shape, int material_index);
        void replace_material(const int from, const int to); // Sets all data octants with material_index of 'from' to 'to'. Passing a negative value will also make isolevel positive (a.k.a. empty space).
        TERRAIN_METHOD get_terrain_type() {return Surface_Nets;}
        RayQueryResult query_ray(const Ray &ray);
        ShapeQueryResult query_shape(const Shape &shape);
        
        static char *serialize(const SurfaceNetsTerrain &terrain, int &char_count);
        static SurfaceNetsTerrain *deserialize(const char *data, const int char_count);
    };
}

#endif