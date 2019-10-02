#include "cell.hpp"
#include <assert.h>
#include "../cube_vertices.hpp"

namespace bigrock {

    Cell::Cell() : children(nullptr)
    {
        for(int i = 0; i < 8; i++)
            corners[i].reset(new Point());
    }

    Point &Cell::get_corner(unsigned char index)
    {
        assert(index < 8);
        return *corners[index];
    }

    const Point &Cell::get_corner(unsigned char index) const
    {
        assert(index < 8);
        return *corners[index];
    }

    Cell *Cell::get_child(unsigned char index)
    {
        assert(index < 8);
        if(is_leaf())
            return nullptr;
        
        return &children[index];
    }

    const Cell *Cell::get_child(unsigned char index) const
    {
        assert(index < 8);
        if(is_leaf())
            return nullptr;
        
        return &children[index];
    }

    void Cell::subdivide()
    {
        if(has_children())
            return;
        
        /* Step 1. Setup new Point corners for children
         *
         * Point Construction Diagram
         * 
         * Number = index in new_points (construction is out of order)
         * (Number) = index in corners
         *(6)-----------------18------------------(7)
         * |\                                      |\
         * | \                                     | \
         * |  \                                    |  \
         * |   \                                   |   \
         * |   11-------------------12-----------------13
         * |    |\                                 |    |\
         * |    | \                                |    | \
         * 15---|--\----------16------------------17    |  \
         * |    |   \                              |    |   \
         * |    |    \                             |    |    \
         * |    |    (2)-------------------4-----------------(3)
         * |    |     |                            |    |     |
         * |    8-----|--------------9-----------------10     |
         *(4)---|-----|-------14------------------(5)   |     |
         *  \   |     |                             \   |     |
         *   \  |     |                              \  |     |
         *    \ |     |                               \ |     |
         *     \|     1--------------------2------------------3
         *      5-----|--------------6------------------7     |
         *       \    |                                  \    |
         *        \   |                                   \   |
         *         \  |                                    \  |
         *          \ |                                     \ |
         *           \|                                      \|
         *           (0)-------------------0-----------------(1)
         * 
         * It's important to construct each point with combined data from other points,
         * otherwise the resulting point may be inaccurate. For example, constructing
         * point 2 from corners (0) and (3) would completely ignore the data of corners
         * (1) and (2). The correct order would be to get 1 from (0) and (2), 3 from (1)
         * and (3), then construct 2 from 1 and 3. That way, the resulting point includes
         * data from corners (1), (2), (3), and (4).
         */

        std::shared_ptr<Point> new_points[19];
        // Construct -z plane
        new_points[0].reset(new Point(corners[0]->lerp(*corners[1], 0.5f)));
        new_points[1].reset(new Point(corners[0]->lerp(*corners[2], 0.5f)));
        new_points[3].reset(new Point(corners[1]->lerp(*corners[3], 0.5f)));
        new_points[2].reset(new Point(new_points[1]->lerp(*new_points[3], 0.5f)));
        new_points[4].reset(new Point(corners[2]->lerp(*corners[3], 0.5f)));

        // Construct +z plane
        new_points[14].reset(new Point(corners[4]->lerp(*corners[5], 0.5f)));
        new_points[15].reset(new Point(corners[4]->lerp(*corners[6], 0.5f)));
        new_points[17].reset(new Point(corners[5]->lerp(*corners[7], 0.5f)));
        new_points[16].reset(new Point(new_points[15]->lerp(*new_points[17], 0.5f)));
        new_points[18].reset(new Point(corners[6]->lerp(*corners[7], 0.5f)));


        // Construct x planes
        // -x
        new_points[5].reset(new Point(corners[0]->lerp(*corners[4], 0.5f)));
        new_points[11].reset(new Point(corners[2]->lerp(*corners[6], 0.5f)));
        // +x
        new_points[7].reset(new Point(corners[1]->lerp(*corners[5], 0.5f)));
        new_points[13].reset(new Point(corners[3]->lerp(*corners[7], 0.5f)));

        // Construct midpoints
        new_points[6].reset(new Point(new_points[5]->lerp(*new_points[7], 0.5f)));
        new_points[8].reset(new Point(new_points[5]->lerp(*new_points[11], 0.5f)));
        new_points[10].reset(new Point(new_points[7]->lerp(*new_points[13], 0.5f)));
        new_points[12].reset(new Point(new_points[11]->lerp(*new_points[13], 0.5f)));
        new_points[9].reset(new Point(new_points[8]->lerp(*new_points[10], 0.5f)));

        // Step 2: Assign points to children

        children = (Cell*)malloc(sizeof(Cell) * 8);
        for(int i = 0; i < 8; i++)
            new (children + i) (Cell) (depth + 1); // Placement new
        
        // Assign points using the diagram above
        children[0].corners[0] = corners[0];
        children[0].corners[1] = new_points[0];
        children[0].corners[2] = new_points[1];
        children[0].corners[3] = new_points[2];
        children[0].corners[4] = new_points[5];
        children[0].corners[5] = new_points[6];
        children[0].corners[6] = new_points[8];
        children[0].corners[7] = new_points[9];

        children[1].corners[0] = new_points[0];
        children[1].corners[1] = corners[1];
        children[1].corners[2] = new_points[2];
        children[1].corners[3] = new_points[3];
        children[1].corners[4] = new_points[6];
        children[1].corners[5] = new_points[7];
        children[1].corners[6] = new_points[9];
        children[1].corners[7] = new_points[10];

        children[2].corners[0] = new_points[1];
        children[2].corners[1] = new_points[2];
        children[2].corners[2] = corners[2];
        children[2].corners[3] = new_points[4];
        children[2].corners[4] = new_points[8];
        children[2].corners[5] = new_points[9];
        children[2].corners[6] = new_points[11];
        children[2].corners[7] = new_points[12];

        children[3].corners[0] = new_points[2];
        children[3].corners[1] = new_points[3];
        children[3].corners[2] = new_points[4];
        children[3].corners[3] = corners[3];
        children[3].corners[4] = new_points[9];
        children[3].corners[5] = new_points[10];
        children[3].corners[6] = new_points[12];
        children[3].corners[7] = new_points[13];

        children[4].corners[0] = new_points[5];
        children[4].corners[1] = new_points[6];
        children[4].corners[2] = new_points[8];
        children[4].corners[3] = new_points[9];
        children[4].corners[4] = corners[4];
        children[4].corners[5] = new_points[14];
        children[4].corners[6] = new_points[15];
        children[4].corners[7] = new_points[16];

        children[5].corners[0] = new_points[6];
        children[5].corners[1] = new_points[7];
        children[5].corners[2] = new_points[9];
        children[5].corners[3] = new_points[10];
        children[5].corners[4] = new_points[14];
        children[5].corners[5] = corners[5];
        children[5].corners[6] = new_points[16];
        children[5].corners[7] = new_points[17];

        children[6].corners[0] = new_points[8];
        children[6].corners[1] = new_points[9];
        children[6].corners[2] = new_points[11];
        children[6].corners[3] = new_points[12];
        children[6].corners[4] = new_points[15];
        children[6].corners[5] = new_points[16];
        children[6].corners[6] = corners[6];
        children[6].corners[7] = new_points[18];

        children[7].corners[0] = new_points[9];
        children[7].corners[1] = new_points[10];
        children[7].corners[2] = new_points[12];
        children[7].corners[3] = new_points[13];
        children[7].corners[4] = new_points[16];
        children[7].corners[5] = new_points[17];
        children[7].corners[6] = new_points[18];
        children[7].corners[7] = corners[7];

        // All done
    }

    void Cell::undivide()
    {
        if(is_leaf())
            return;
        
        for(int i = 0; i < 8; i++)
            children[i].~Cell();
        
        free(children);
        children = nullptr;
    }

    unsigned char Cell::get_child_with_point(glm::vec3 point)
    {
        unsigned char result = 0;
        if(point.x > 0.5f)
            result |= 1;
        if(point.y > 0.5f)
            result |= 2;
        if(point.z > 0.5f)
            result |=  4;
        return result;
    }

    void Cell::apply(const Shape &s, const Action &a, unsigned char max_depth)
    {
        // Check depth and subdivide if needed
        if(is_leaf() && depth < max_depth)
        {
            AABB aabb;
            aabb.extend(glm::vec3(0,0,0));
            aabb.extend(glm::vec3(1,1,1));
            if(s.is_convex())
            {
                if(s.get_surface_aabb().intersect(aabb) == AABB::INTERSECT) // Crosses isosurface
                    this->subdivide();
            }
            else
            {
                if(s.get_surface_aabb().intersect(aabb) != AABB::OUTSIDE)
                    this->subdivide();
            }
        }

        // To avoid double-applications, we only apply to Points on leaf cells
        // Application ignores max_depth
        if(has_children())
        {
            for(int i = 0; i < 8; i++)
            {
                std::unique_ptr<Shape> shape_copy = s.duplicate(); // TODO: Shape copy pooling?
                shape_copy->transform.scale *= 2; // Child cells are half size, so shapes become twice as big relatively
                shape_copy->transform.origin *= 2;
                shape_copy->transform.origin -= CUBE_VERTICES[i] / float(unsigned long(1) << (depth + 1));
                get_child(i)->apply(*shape_copy, a, max_depth);
            }
        }
        else
        {
            for(int i = 0; i < 8; i++)
                a.update(s, CUBE_VERTICES[i], *corners[i]);
        }
    }

    PointQuery Cell::query_point(glm::vec3 point) const
    {
        bool point_valid = glm::all(glm::greaterThanEqual(point, glm::vec3(0,0,0))) && glm::all(glm::lessThanEqual(point, glm::vec3(1,1,1))); // all components of point are >= 0 and <= 1
        
        assert(point_valid);
        if(!point_valid)
            return PointQuery();

        if(has_children())
        {
            unsigned char next_index = get_child_with_point(point);
            return get_child(next_index)->query_point(point);
        }
        else
        {
            // Interpolate
            Point x_plane[4] = { // Squash the cube into a plane
                corners[0]->lerp(*corners[1], point.x),
                corners[2]->lerp(*corners[3], point.x),
                corners[4]->lerp(*corners[5], point.x),
                corners[6]->lerp(*corners[7], point.x)
            };

            Point y_line[2] = { // Squash the plane into a line
                x_plane[0].lerp(x_plane[1], point.y),
                x_plane[2].lerp(x_plane[3], point.y)
            };

            Point result = y_line[0].lerp(y_line[1], point.z); // Squash the line into a Point
            return PointQuery(result.density, result.material, depth);
        }
    }
}