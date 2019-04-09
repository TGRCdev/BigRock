#include "cell.hpp"

#include "../grid_vertices.hpp"

#include <set>

namespace bigrock {
namespace terrain {
namespace data {

Cell::Cell(const unsigned int &subdiv_level)
{
    this->subdiv_level = subdiv_level;
    // Vertices don't need to be set, this is the parent's job
    children = NULL;
    owned_vertices = 0x00;
}

Cell::Cell()
{
    this->subdiv_level = 0;
    for(int i = 0; i < 8; i++)
        vertices[i] = new CellVertex(GRID_VERTICES[i]);
    owned_vertices = 0xFF;
    children = NULL;
}

Cell::~Cell()
{
    if(!is_leaf())
        undivide();
    
    for(int i = 0; i < 8; i++)
    {
        if((owned_vertices & (1 << i)) != 0)
            delete vertices[i];
    }
}

bool Cell::can_collapse() const
{
    if (!is_leaf())
        return false;

    bool collapse = true;

    for(int i = 0; collapse && i < 7; i++)
    {
        collapse = collapse && vertices[i]->can_collapse(*vertices[i+1]); // Collapsibility is transitive
    }

    return collapse;
}

void Cell::subdivide()
{
    if (!is_leaf())
        return;
    
    // Step 1: Setup vertices
    CellVertex *vertex_pointers[2][2][2];
    std::set<CellVertex*> owned;
    // Prepare existing vertices for sharing
    for(int i = 0; i < 8; i++)
    {
        owned.insert(vertices[i]);
        math::Vector3 p = GRID_VERTICES[i];
        vertex_pointers[int(p.x)][int(p.y)][int(p.z)] = vertices[i];
    }

    for(int x = 0; x < 2; x++)
        for(int y = 0; y < 2; y++)
            for(int z = 0; z < 2; z++)
            {
                if(!(
                    (x == 0 || x == 2) && 
                    (y == 0 || y == 2) && 
                    (z == 0 || z == 2)))
                    vertex_pointers[x][y][z] = new CellVertex(sample_local(math::Vector3(x, y, z) / 2));
            }
    
    // Step 2: Add children
    children = (Cell*)malloc(sizeof(Cell) * 8);
    for(int i = 0; i < 8; i++)
    {
        children[i] = Cell(subdiv_level + 1);
        math::Vector3 cpos = GRID_VERTICES[i];
        for(int j = 0; j < 8; j++)
        {
            math::Vector3 vpos = (GRID_VERTICES[j] + cpos) / 2;
            children[i].vertices[j] = vertex_pointers[int(vpos.x)][int(vpos.y)][int(vpos.z)];
            if(owned.find(children[i].vertices[j]) == owned.end())
            {
                owned.insert(children[i].vertices[j]);
                children[i].owned_vertices |= (1 << i);
            }
        }
    }
}

void Cell::undivide()
{
    if(is_leaf())
        return;
    
    for(int i = 0; i < 8; i++)
    {
        children[i].~Cell();
    }

    free(children);
}

void Cell::try_undivide()
{
    if(is_leaf())
        return;
    
    for(int i = 0; i < 8; i++) // Recursive undivide
        children[i].try_undivide();
    
    bool collapse = true;

    for(int i = 0; collapse && i < 8; i++)
    {
        collapse = collapse && children[i].can_collapse();
    }

    if(collapse)
        undivide();
}

CellVertex Cell::sample(const math::Vector3 &pos) const
{
    math::Vector3 t = (pos - vertices[0]->position) / (vertices[7]->position - vertices[0]->position);
    return sample_local(t);
}

CellVertex Cell::sample_local(const math::Vector3 &pos) const
{
    if(is_leaf())
    {
        CellVertex x_plane[4] = {
            vertices[0]->interpolate(*vertices[1], pos.x),
            vertices[2]->interpolate(*vertices[3], pos.x),
            vertices[4]->interpolate(*vertices[5], pos.x),
            vertices[6]->interpolate(*vertices[7], pos.x)
        };

        CellVertex y_line[2] = {
            x_plane[0].interpolate(x_plane[1], pos.y),
            x_plane[2].interpolate(x_plane[3], pos.y)
        };

        return y_line[0].interpolate(y_line[1], pos.z);
    }
    else
    {
        return children[get_index_containing_local_pos(pos)].sample_local(pos);
    }
}

int Cell::get_index_containing_pos(const math::Vector3 &pos) const
{
    math::Vector3 midpoint = vertices[0]->position.lerp(vertices[7]->position, 0.5);
    int index = 0;
    if(pos.x > midpoint.x)
        index |= 1;
    if(pos.y > midpoint.y)
        index |= 2;
    if(pos.z > midpoint.z)
        index |= 4;
    return index;
}

int Cell::get_index_containing_local_pos(const math::Vector3 &pos) const
{
    int index = 0;
    if(pos.x > 0.5)
        index |= 1;
    if(pos.y > 0.5)
        index |= 2;
    if(pos.z > 0.5)
        index |= 4;
    return index;
}

math::AABB Cell::get_aabb() const
{
    math::AABB ret;
    ret.position = vertices[0]->position;
    ret.size = vertices[7]->position - vertices[0]->position;
    return ret;
}

}}}
