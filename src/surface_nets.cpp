#include "surface_nets.h"
#include "terrain_mesh.h"

#include <exception>
#include <map>

namespace bigrock
{
    #pragma region SNOctreeData
    bool SNOctreeData::can_collapse(const SNOctreeData &other)
    {
        if(this->isoval > 0)
        {
            return other.isoval > 0;
        }
        else
        {
            return other.isoval <= 0 && this->material_index == other.material_index;
        }
    }

    SNOctreeData SNOctreeData::interpolate(const SNOctreeData &other, const float t)
    {
        if(t <= 0)
            return *this;
        else if(t >= 1)
            return other;
        
        SNOctreeData data;
        data.isoval = this->isoval + ((other.isoval - this->isoval) * t);
        if(data.isoval <= 0) // If data.isoval <= 0, material_index should not be -1
        { // Interpolate material index
            if(other.material_index == -1)
                data.material_index = this->material_index;
            else
            {
                float v1 = -fabsf(this->isoval);
                float v2 = fabsf(other.isoval);
                float v3 = v1 + ((v2 - v1) * t);
                if(v3 < 0)
                    data.material_index = this->material_index;
                else
                    data.material_index = other.material_index;
            }
        }
        else
        {
            data.material_index = -1;
        }
        return data;
    }
    #pragma endregion

    #pragma region SNOctree
    SNOctree::SNOctree(SNOctree *parent, SNOctreeRoot *root, const unsigned int &depth, const Vector3 &relative_position, const Vector3 &relative_size, const SNOctreeData &data)
    {
        this->parent = parent;
        this->root = root;
        this->depth = depth;
        this->relative_position = relative_position;
        this->relative_size = relative_size;
        this->is_leaf = true;
        this->data = static_cast<void*>(new SNOctreeData(data));
    }

    SNOctree::SNOctree()
    {
        this->is_leaf = true;
        this->data = static_cast<void*>(new SNOctreeData());
    }

    SNOctree::~SNOctree()
    {
        if(is_leaf)
            delete static_cast<SNOctreeData*>(data);
        else
            delete[] static_cast<SNOctree*>(data);
    }

    SNOctreeData SNOctree::get_data() const
    {
        if(is_leaf)
            return *static_cast<SNOctreeData*>(this->data);
        else
        {
            get_leaf()->get_data();
        }
    }

    void SNOctree::set_data(const SNOctreeData &data)
    {
        if(is_leaf)
        {
            delete static_cast<SNOctreeData*>(this->data);
            this->data = static_cast<void*>(new SNOctreeData(data));
        }
        else
        {
            get_leaf()->set_data(data);
        }
    }

    SNOctree *SNOctree::get_child(const int index)
    {
        if(index < 0 || index > 8)
            throw std::out_of_range("Attempted to get child of SNOctree out of range.");
        if(is_leaf)
            throw std::logic_error("Attempted to get child of leaf SNOctree");
        
        return &static_cast<SNOctree*>(data)[index];
    }

    SNOctree *SNOctree::get_leaf()
    {
        if(is_leaf)
            return this;
        else
        {
            SNOctree *current_octant = this->get_child(0);
            while(!current_octant->is_leaf)
                current_octant = current_octant->get_child(0);
            return current_octant;
        }
    }

    const SNOctree *SNOctree::get_leaf() const
    {
        if(is_leaf)
            return this;
        else
        {
            const SNOctree *current_octant = this->get_child(0);
            while(!current_octant->is_leaf)
                current_octant = current_octant->get_child(0);
            return current_octant;
        }
    }

    bool SNOctree::is_root_node() const
    {
        return this == root;
    }

    int SNOctree::get_index_containing_point(const Vector3 &point) const
    {
        Vector3 halfsize = this->get_size() / 2;
        Vector3 pos = this->get_position();
        Vector3 midpoint = pos + halfsize;

        int index = 0;
        if(point.x > midpoint.x)
            index |= 1;
        if(point.y > midpoint.y)
            index |= 2;
        if(point.z > midpoint.z)
            index |= 4;
        return index;
    }

    bool SNOctree::contains_point(const Vector3 &point) const
    {
        Vector3 origin = this->get_position();
        Vector3 size = this->get_size();
        Vector3 bounds = origin + size;
        return (point.x >= origin.x && point.y >= origin.y && point.z >= origin.z) && (point.x <= bounds.x && point.y <= bounds.y && point.z <= bounds.z);
    }

    SNOctreeData SNOctree::get_interp_data(const Vector3 &point) const
    {
        if(this->is_leaf)
            return *static_cast<SNOctreeData*>(this->data);
        
        int index = this->get_index_containing_point(point);
        const SNOctree *child = this->get_child(index);
        if(get_child(index)->is_leaf)
        { // We can interpolate!
            SNOctreeData x1, x2, x3, x4;
            Vector3 pos = get_position();
            Vector3 size = get_size();

            Vector3 t = (point - pos) / (size);

            SNOctreeData x1, x2, x3, x4; // Flatten data on the x axis to create a plane of data
            x1 = get_child(0)->get_data().interpolate(get_child(1)->get_data(), t.x);
            x2 = get_child(2)->get_data().interpolate(get_child(3)->get_data(), t.x);
            x3 = get_child(4)->get_data().interpolate(get_child(5)->get_data(), t.x);
            x4 = get_child(6)->get_data().interpolate(get_child(7)->get_data(), t.x);

            SNOctreeData y1, y2; // Flatten data on the y axis to create a line segment of data
            y1 = x1.interpolate(x2, t.y);
            y2 = x3.interpolate(x4, t.y);

            SNOctreeData result; // Interpolate on the Z axis to get the result
            result = y1.interpolate(y2, t.z);

            return result;
        }
        else
        { // TODO: Iterative?
            return child->get_interp_data(point);
        }
    }

    SNOctree *SNOctree::get_closest_octant(const Vector3 &point)
    {
        SNOctree *current_octant = this;
        while(!current_octant->is_leaf)
            current_octant = current_octant->get_child(current_octant->get_index_containing_point(point));
        return current_octant;
    }

    const SNOctree *SNOctree::get_closest_octant(const Vector3 &point) const
    {
        const SNOctree *current_octant = this;
        while(!current_octant->is_leaf)
            current_octant = current_octant->get_child(current_octant->get_index_containing_point(point));
        return current_octant;
    }

    void SNOctree::subdivide()
    {
        if(!is_leaf)
            return;
        
        delete static_cast<SNOctreeData*>(this->data);
        this->is_leaf = false;
        SNOctree *children = new SNOctree[8];
        for(int i = 0; i < 8; i++)
        {
            children[i].parent = this;
            children[i].root = this->root;
            children[i].depth = this->depth + 1;
            children[i].relative_position = this->relative_position + (get_grid_vertex(i) / (this->depth + 2));
            children[i].relative_size = Vector3(1,1,1) / (this->depth + 2);
            children[i].is_leaf = true;
            children[i].set_data(*static_cast<SNOctreeData*>(this->data));
        }
        this->data = static_cast<void*>(children);
    }

    void SNOctree::collapse()
    {
        if(is_leaf)
            return;
        
        SNOctreeData *new_data = new SNOctreeData(this->get_leaf()->get_data());
        delete[] static_cast<SNOctree*>(this->data);
        this->is_leaf = true;
        this->data = static_cast<void*>(new_data);
    }

    bool SNOctree::can_collapse()
    {
        if(is_leaf)
            return false;
        
        for(int i = 1; i < 8; i++)
        {
            SNOctree *child1 = get_child(i-1);
            SNOctree *child2 = get_child(i);
            
            if(!child1->is_leaf || !child2->is_leaf)
                return false;
            else if(!child1->get_data().can_collapse(child2->get_data()))
                return false;
        }
        return true;
    }

    void SNOctree::optimize()
    {
        if(is_leaf)
            return;
    
        bool collapse = true;
        for(int i = 0; i < 8; i++)
        {
            SNOctree *child = get_child(i);
            if(!child->is_leaf)
                child->optimize();
            if(!child->is_leaf)
                collapse = false;
        }

        if(collapse && can_collapse())
            this->collapse();
    }

    Vector3 SNOctree::get_position() const
    {
        if(this->is_root_node())
            return Vector3(0,0,0);
        else
            return this->relative_position * root->get_size();
    }

    Vector3 SNOctree::get_relative_position() const
    {
        if(this->is_root_node())
            return Vector3(0,0,0);
        else
            return this->relative_position;
    }

    Vector3 SNOctree::get_size() const
    {
        if(this->is_root_node())
            return this->root->size;
        else
            return this->relative_size * this->root->size;
    }

    Vector3 SNOctree::get_relative_size() const
    {
        if(this->is_root_node())
            return Vector3(1,1,1);
        else
            return this->relative_size;
    }
    #pragma endregion

    #pragma region SNOctreeRoot
    SNOctreeRoot::SNOctreeRoot(const Vector3 &size)
    {
        this->root = this;
        this->parent = NULL;
        this->depth = 0;
        this->size = size;
        this->is_leaf = true;
        this->data = static_cast<void*>(new SNOctreeData());
    }
    #pragma endregion

    #pragma region SurfaceNetsTerrain
    void SurfaceNetsTerrain::create_mesh()
    {
        if(this->mesh != NULL)
            return;
        
        this->mesh = new TerrainMesh();
        this->update_mesh(true);
    }

    void SurfaceNetsTerrain::update_mesh(bool force_refresh)
    {
        polygonise(&this->octree);
        mesh_needs_update = false;
    }

    unsigned int SurfaceNetsTerrain::polygonise(SNOctree *octant)
    {
        if(octant->is_leaf_node())
            return octant->get_depth();
        
        // First, we polygonise any non-leaf children and get
        // the lowest subdivision level
        unsigned int lowest_depth = octant->get_depth();
        for(int i = 0; i < 8; i++)
        {
            SNOctree *child = octant->get_child(i);
            if(!child->is_leaf_node())
            {
                unsigned int dep = polygonise(child);
                if (dep < lowest_depth)
                    lowest_depth = dep;
            }
        }

        unsigned int subdiv = lowest_depth - octant->get_depth();
        unsigned int count = 2 << subdiv;

        // All children are either leaf nodes or polygonised.
        
        for(int x = 0; x < count; x++)
        {
            for(int y = 0; y < count; y++)
            {
                for(int z = 0; z < count; z++)
                {
                    
                }
            }
        }
    }
    #pragma endregion
}