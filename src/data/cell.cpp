#include "cell.hpp"

#include "../grid_vertices.hpp"
#include "../defines.hpp"

namespace bigrock {
namespace data {

using namespace flatbuffers;

Cell::Cell(bool init_root)
{
    children = NULL;

    if(init_root)
    {
        for(int i = 0; i < 8; i++)
            corners[i] = new Point();
        
        owned_vertices = 0xFF;
        cell_count = new unsigned int(1); // Itself!
    }
    else
    {
        for(int i = 0; i < 8; i++)
            corners[i] = NULL;
        owned_vertices = 0;
        cell_count = NULL;
    }
    this->position = glm::vec3(0.0f,0.0f,0.0f);
    subdiv_level = 0;
}

void Cell::load(const schemas::Cell &cell, PointMap &points)
{
    this->owned_vertices = cell.owned_vertices();
    auto v = cell.position();
    this->position = glm::vec3(v->x(), v->y(), v->z());
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
    if(data_children->size() == 8 && this->subdiv_level + 1 < BR_MAX_CELL_DEPTH)
    {
        children = static_cast<Cell*>(malloc(sizeof(Cell) * 8));
        for(int i = 0; i < 8; i++)
            new (children + i) (Cell) (*data_children->Get(i), points, this->cell_count, this->subdiv_level + 1, this->position + (GRID_VERTICES[i] / float(1ULL << (subdiv_level + 1)))); // Placement new

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
    this->position = glm::vec3();
    this->cell_count = new unsigned int(1);
    this->subdiv_level = 0;
    this->load(cell, p);
}

Cell::Cell(const schemas::Cell &cell, PointMap &p, unsigned int *cell_count, unsigned char subdiv_level, glm::vec3 position)
{
    this->cell_count = cell_count;
    this->subdiv_level = subdiv_level;
    this->position = position;
    this->load(cell, p);
}

Cell::~Cell()
{
    if(has_children())
        undivide();
    
    for(int i = 0; i < 8; i++)
        if((owned_vertices & (1 << i)) != 0)
            delete corners[i];

    if(subdiv_level == (unsigned char)(0xFF))
        delete cell_count;
}

void Cell::subdivide()
{
    if(has_children() || subdiv_level + 1 >= BR_MAX_CELL_DEPTH)
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
        children[i].position = this->position + (GRID_VERTICES[i] / float(1ULL << (subdiv_level + 1)));
        glm::vec<3, int> cpos = GRID_VERTICES[i];
        
        #ifndef NDEBUG
        assert(!glm::any(glm::greaterThan(children[i].position, glm::vec3(1.0f))));
        #endif

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

bool Cell::optimize()
{
    if(is_leaf())
        return can_collapse();
    
    bool can_optimize = true;
    for(int i = 0; i < 8; i++)
        can_optimize = children[i].optimize() && can_optimize;
    
    if(can_optimize)
        undivide();
    
    return can_optimize && can_collapse();
}

bool Cell::can_collapse() const
{
    bool collapse = true;
    for(int i = 0; collapse && i < 7; i++)
        collapse = collapse && corners[i]->can_collapse(*corners[i+1]); // Collapsibility is transitive
    return collapse;
}

int Cell::get_index_containing_pos(const glm::vec3 pos) const
{
    glm::vec3 midpoint = glm::mix(get_corner_pos(0), get_corner_pos(7), 0.5);
    int index = 0;

    if(pos.x > midpoint.x)
        index |= 1;
    if(pos.y > midpoint.y)
        index |= 2;
    if(pos.z > midpoint.z)
        index |= 4;

    return index;
}

Point Cell::sample(glm::vec3 point, bool recursive) const
{
    if(has_children() && recursive)
    {
        return children[get_index_containing_pos(point)].sample(point, true);
    }
    else
    {
        glm::vec3 t = (point - position) / (this->get_corner_pos(7) - position);
        return sample_local(t);
    }
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

AABB Cell::get_aabb() const
{
    AABB ret;
    ret.extend(get_corner_pos(0));
    ret.extend(get_corner_pos(7));
    return ret;
}

#if !BR_DISABLE_MULTITHREADING

struct cell_apply_data
{
    Cell *c;
    const Tool *t;
    const Action *a;
    int max_depth;
};

int Cell::cell_apply_thread(void *userdata)
{ // userdata is the Cell
    if(!userdata)
        return 1;
    
    cell_apply_data *data = reinterpret_cast<cell_apply_data*>(userdata);
    if(!data->c || !data->t || !data->a)
        return 1;
    
    data->c->apply_threaded(*data->t, *data->a, data->max_depth);
    delete data;
    return 1;
}

bool pool_ready = false;
JobPool *pool = reinterpret_cast<JobPool*>(malloc(sizeof(JobPool)));
#endif
void Cell::apply(const Tool &t, const Action &a, const int max_depth, bool multithread, unsigned char thread_count)
{
    #if !BR_DISABLE_MULTITHREADING
    if(multithread)
    {
        if(pool_ready)
            pool->~JobPool();
        new (pool) (JobPool) (thread_count);
        pool_ready = true;

        cell_apply_data *root_data = new cell_apply_data();
        root_data->t = &t;
        root_data->a = &a;
        root_data->c = this;
        root_data->max_depth = max_depth;
        pool->add_job(&cell_apply_thread, root_data);
        pool->wait_until_empty();
    }
    else
    {
        this->apply_unthreaded(t, a, max_depth);
    }
    #else
        this->apply_unthreaded(t, a, max_depth);
    #endif
}

bool check_subdivide(Cell &cell, const Tool &t, const int max_depth)
{
    if(cell.has_children())
        return true;
    else
    {
        if(max_depth != -1 && cell.get_depth() >= max_depth)
            return false;
        
        if(t.is_concave())
        {
            if(t.get_max_depth() == -1 || t.get_max_depth() > cell.get_depth())
            {
                cell.subdivide();
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if(t.get_aabb().intersect(cell.get_aabb()) != AABB::OUTSIDE)// && (t.value(cell.get_corner_pos(0)) < 0.00001 || t.value(cell.get_corner_pos(7)) < 0.00001)) // Only need surface detail for convex shapes
            {
                cell.subdivide();
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

#if !BR_DISABLE_MULTITHREADING

// More effective on 3 or more core machines
void Cell::apply_threaded(const Tool &t, const Action &a, const int max_depth)
{
    bool apply_children = check_subdivide(*this, t, max_depth);
    
    for(int i = 0; i < 8; i++)
        if(owns_corner(i))
            a.update(t, get_corner_pos(i), get_corner(i));
    
    if(apply_children)
    {
        JobPool::job_t jobs[8];
        int job_count = 0;
        for(int i = 0; i < 8; i++)
        {
            if(children[i].get_aabb().intersect(t.get_aabb()) != AABB::OUTSIDE)
            {
                cell_apply_data *thread_data = new cell_apply_data();
                thread_data->c = children + i;
                thread_data->t = &t;
                thread_data->a = &a;
                thread_data->max_depth = max_depth;
                jobs[job_count].func = &cell_apply_thread;
                jobs[job_count].userdata = thread_data;
                job_count++;
            }
        }
        return pool->add_jobs(jobs, job_count);
    }
}

#endif

// More effective on 1-2 core machines
void Cell::apply_unthreaded(const Tool &t, const Action &a, const int max_depth)
{
    bool apply_children = check_subdivide(*this, t, max_depth);
    
    for(int i = 0; i < 8; i++)
        if(owns_corner(i))
            a.update(t, get_corner_pos(i), get_corner(i));
    
    if(apply_children)
    {
        for(int i = 0; i < 8; i++)
        {
            if(t.get_aabb().intersect(children[i].get_aabb()) != AABB::OUTSIDE)
                children[i].apply_unthreaded(t, a, max_depth);
        }
    }
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
            Offset<schemas::Point> p_off = schemas::CreatePoint(builder, 
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

    if(has_children() && subdiv_level < (FLATBUFFERS_MAX_PARSING_DEPTH - 2))
    {
        child_offsets.reserve(8);
        for(int i = 0; i < 8; i++)
            child_offsets.push_back(children[i].serialize(builder, point_offsets));
    }

    auto v = schemas::Vec3(this->position.x, this->position.y, this->position.z);

    return schemas::CreateCellDirect(builder,
        &v,
        &offsets,
        &child_offsets,
        owned_vertices
    );
}

std::unique_ptr<std::string> Cell::serialize() const
{
    std::map<const Point*, Offset<schemas::Point> > point_offsets;
    FlatBufferBuilder builder((*cell_count * sizeof(schemas::Cell)) + sizeof(schemas::Cell));
    auto cell_offs = serialize(builder, point_offsets);
    builder.Finish(cell_offs);
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

unsigned char Cell::get_cube_index() const
{
    unsigned char index = 0;
    if(corners[0]->isovalue > 0) index |= 1;
    if(corners[1]->isovalue > 0) index |= 2;
    if(corners[2]->isovalue > 0) index |= 4;
    if(corners[3]->isovalue > 0) index |= 8;
    if(corners[4]->isovalue > 0) index |= 16;
    if(corners[5]->isovalue > 0) index |= 32;
    if(corners[6]->isovalue > 0) index |= 64;
    if(corners[7]->isovalue > 0) index |= 128;
    return index;
}

}}