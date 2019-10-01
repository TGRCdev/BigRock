#include "cell.hpp"
#include <assert.h>

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
}