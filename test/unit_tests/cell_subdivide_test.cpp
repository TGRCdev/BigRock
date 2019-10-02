#include <data/cell.hpp>
#include <assert.h>
#include <set>

using namespace bigrock;

#include "../shared.hpp"

int main()
{
    printf("\nCell Subdivide/Undivide Test\n\n");

    Cell c;
    printf("Cell created\n");

    printf("Has children: %s\n", BOOL2STR(c.has_children()));
    printf("Expected: false\n");
    assert(!c.has_children());

    printf("\nSubdividing the cell...");
    c.subdivide();
    printf("Done.\n");
    printf("Has children: %s\n", BOOL2STR(c.has_children()));
    printf("Expected: true\n");
    assert(c.has_children());

    printf("\nChecking that all children and child corners are valid...");
    for(int i = 0; i < 8; i++) // For each child cell
    {
        assert(c.get_child(i)); // Child is valid

        std::set<Point*> checked_points;
        for(int j = 0; j < 8; j++) // For each corner
        {
            // We are testing two things
            // 1. All corners are valid (not nullptr)
            // 2. Each corner in a child is distinct
            Point* corner = &c.get_child(i)->get_corner(j);

            assert(corner); // Corner is valid
            assert(checked_points.find(corner) == checked_points.end()); // Corner is distinct... so far
            checked_points.insert(corner);
        }
    }
    printf("Done\n");

    printf("\nUndividing Cell...");
    c.undivide();
    printf("Done\n");
    printf("Has children: %s\n", BOOL2STR(c.has_children()));
    printf("Expected: false\n");
    assert(!c.has_children());
    assert(!c.get_child(0));
}