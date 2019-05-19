#include "cell.hpp"

#include "../grid_vertices.hpp"

namespace bigrock {
namespace data {

Cell::Cell(bool init_root)
{
    children = NULL;

    if(init_root)
    {
        for(int i = 0; i < 8; i++)
        {
            corners[i] = new Point();
            corners[i]->position = GRID_VERTICES[i];
        }
        owned_vertices = ~(char(0));
    }
    else
    {
        for(int i = 0; i < 8; i++)
            corners[i] = NULL;
        owned_vertices = 0;
    }
    subdiv_level = 0;
}

Cell::~Cell()
{
    if(has_children())
    {
        undivide();
    }
    
    for(int i = 0; i < 8; i++)
    {
        delete corners[i];
        corners[i] = NULL;
    }
    owned_vertices = 0;
}

void Cell::subdivide()
{
    if(has_children())
        return;
    
    Point *vertex_pointers[3][3][3];
    bool owned[3][3][3];

    // Step 1: Setup shared vertices
    for(int i = 0; i < 8; i++)
    {
        glm::vec<3, int> pos = glm::vec<3, int>(GRID_VERTICES[i]) * 2;
        owned[pos.x][pos.y][pos.z] = true;
        vertex_pointers[pos.x][pos.y][pos.z] = this->corners[i];
    }

    // Step 2: Construct new vertices
    for(int x = 0; x < 3; x++)
    for(int y = 0; y < 3; y++)
    for(int z = 0; z < 3; z++)
    {
        if(!owned[x][y][z])
        {
            Point *pt = new Point();
            *pt = sample_local(glm::vec<3, float>(x, y, z) / 2.0f);
            vertex_pointers[x][y][z] = pt;
        }
    }

    // Step 3: Add children
    children = static_cast<Cell*>(calloc(8, sizeof(Cell)));
    for(int i = 0; i < 8; i++)
    {
        children[i] = Cell(false);
        children[i].subdiv_level = this->subdiv_level + 1;
        glm::vec<3, int> cpos = GRID_VERTICES[i];
        for(int j = 0; j < 8; j++)
        {
            glm::vec<3, int> vpos = cpos + glm::vec<3, int>(GRID_VERTICES[j]);
            children[i].corners[j] = vertex_pointers[vpos.x][vpos.y][vpos.z];
            if(!owned[vpos.x][vpos.y][vpos.z])
            {
                children[i].owned_vertices |= (1 << j);
                owned[vpos.x][vpos.y][vpos.z] = true;
            }
        }
    }
}

void Cell::undivide()
{
    if(is_leaf())
        return;
    
    free(children);
    children = NULL;
}

Point Cell::sample(glm::vec3 point) const
{
    glm::vec3 t = (point - corners[0]->position) / (corners[7]->position - corners[0]->position);
    return sample_local(t);
}

Point Cell::sample_local(glm::vec3 point) const
{
    Point x_plane[4];
    x_plane[0] = this->corners[0]->interpolate(*this->corners[1], point.x);
    x_plane[1] = this->corners[2]->interpolate(*this->corners[3], point.x);
    x_plane[2] = this->corners[4]->interpolate(*this->corners[5], point.x);
    x_plane[3] = this->corners[6]->interpolate(*this->corners[7], point.x);

    Point y_plane[2];
    y_plane[0] = x_plane[0].interpolate(x_plane[1], point.y);
    y_plane[1] = x_plane[2].interpolate(y_plane[3], point.y);

    return y_plane[0].interpolate(y_plane[1], point.z);
}

}}