#ifndef BIGROCK_TESTS_SHARED_H
#define BIGROCK_TESTS_SHARED_H

namespace bigrock
{

#ifdef BIGROCK_POINT_H
void print_point(Point p)
{
    printf("{material: %u, density: %f}", p.material, p.density);
}
#endif

}

#endif