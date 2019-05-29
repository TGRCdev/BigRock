#include "cell.hpp"

#include "../grid_vertices.hpp"

#include <iostream>

namespace bigrock {
namespace data {

using namespace flatbuffers;

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
        cell_count = new unsigned int(1); // Itself!
    }
    else
    {
        for(int i = 0; i < 8; i++)
            corners[i] = NULL;
        owned_vertices = 0;
        cell_count = NULL;
    }
    subdiv_level = 0;
}

void Cell::load(const schemas::Cell &cell, PointMap &points)
{
    this->owned_vertices = cell.owned_vertices();
    const Vector<Offset<schemas::Point> > *data_points = cell.corners();
    for(int i = 0; i < 8; i++)
    {
        auto it = points.find(data_points->Get(i));
        Point *p;

        if(it == points.end())
        {
            p = new Point(*data_points->Get(i));
            points.insert(std::make_pair(data_points->Get(i), p));
        }
        else
        {
            p = it->second;
        }

        corners[i] = p;
    }

    // Children can be 8 or 0
    const Vector<Offset<schemas::Cell>> *data_children = cell.children();
    if(data_children->size() == 8) // GOTTA be 8
    {
        children = static_cast<Cell*>(malloc(sizeof(Cell) * 8));
        for(int i = 0; i < 8; i++)
        {
            new (children + i) (Cell) (*data_children->Get(i), points, this->cell_count); // Placement new
            children[i].cell_count = this->cell_count;
        }
        *cell_count += 8;
    }
    else
    {
        children = NULL;
    }
}

Cell::Cell(const schemas::Cell &cell)
{
    PointMap p;
    this->cell_count = new unsigned int(1);
    this->load(cell, p);
    std::cout << "Total points: " << p.size() << std::endl;
}

Cell::Cell(const schemas::Cell &cell, PointMap &p, unsigned int *cell_count)
{
    this->cell_count = cell_count;
    this->load(cell, p);
}

Cell::~Cell()
{
    if(has_children())
        undivide();
    
    for(int i = 0; i < 8; i++)
        if(owned_vertices & (1 << i) != 0)
            delete corners[i];

    if(subdiv_level == ~(0))
        delete cell_count;
}

void Cell::subdivide()
{
    if(has_children())
        return;
    
    Point *vertex_pointers[3][3][3];
    bool owned[3][3][3];

    for(int x = 0; x < 3; x++)
    for(int y = 0; y < 3; y++)
    for(int z = 0; z < 3; z++)
        owned[x][y][z] = false; // Make sure

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
    children = static_cast<Cell*>(malloc(sizeof(Cell) * 8));
    for(int i = 0; i < 8; i++)
    {
        new (children + i) (Cell) (false); // Placement new
        children[i].subdiv_level = this->subdiv_level + 1;
        children[i].cell_count = this->cell_count;
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

    for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
    for(int k = 0; k < 3; k++)
    assert(owned[i][j][k]);

    *cell_count += 8;
}

void Cell::undivide()
{
    if(is_leaf())
        return;

    for(int i = 0; i < 8; i++)
        children[i].~Cell();

    free(children);
    children = NULL;
    *cell_count -= 8;
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
    y_plane[1] = x_plane[2].interpolate(x_plane[3], point.y);

    return y_plane[0].interpolate(y_plane[1], point.z);
}

Offset<schemas::Cell> Cell::serialize(FlatBufferBuilder &builder, std::map<const Point*, Offset<schemas::Point> > &point_offsets) const
{
    std::vector<Offset<schemas::Point>> offsets;
    offsets.reserve(8);

    for(int i = 0; i < 8; i++)
    {
        if(!corners[i])
            continue;
        
        auto it = point_offsets.find(corners[i]);
        Offset<schemas::Point> offs;
        if(it == point_offsets.end())
        {
            auto v = schemas::Vec3(corners[i]->position.x, corners[i]->position.y, corners[i]->position.z);
            Offset<schemas::Point> p_off = schemas::CreatePoint(builder, 
                &v,
                corners[i]->isovalue,
                corners[i]->material
            );

            point_offsets.insert(std::make_pair(corners[i], p_off));
            offs = p_off;
        }
        else
        {
            offs = it->second;
        }
        offsets.push_back(offs);
    }

    std::vector<Offset<schemas::Cell> > child_offsets;

    if(has_children())
    {
        child_offsets.reserve(8);
        for(int i = 0; i < 8; i++)
            child_offsets.push_back(children[i].serialize(builder, point_offsets));
    }

    return schemas::CreateCellDirect(builder,
        &offsets,
        &child_offsets,
        owned_vertices
    );
}

std::unique_ptr<std::string> Cell::serialize() const
{
    std::map<const Point*, Offset<schemas::Point> > point_offsets;
    FlatBufferBuilder builder(*cell_count * sizeof(schemas::Cell));
    auto offs = serialize(builder, point_offsets);
    builder.Finish(offs);
    return std::unique_ptr<std::string>(new std::string(reinterpret_cast<char*>(builder.GetBufferPointer()), builder.GetSize()));
}

std::unique_ptr<Cell> Cell::deserialize(const void *buf, size_t length)
{
    if(buf == NULL)
        return nullptr;
    
    Verifier v = Verifier(reinterpret_cast<const uint8_t*>(buf), length);
    bool ok = schemas::VerifyCellBuffer(v);
    if(!ok)
        return nullptr;
    
    return std::unique_ptr<Cell>(new Cell(*schemas::GetCell(buf)));
}

}}