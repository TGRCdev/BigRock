#include <stdio.h>

#ifndef SHARED_PRINT_VEC3
#include <glm/vec3.hpp>
#endif

namespace bigrock
{

#if defined(BIGROCK_POINT_H) && !defined(SHARED_PRINT_POINT)
#define SHARED_PRINT_POINT
void print_point(Point p)
{
    printf("{material: %u, density: %f}", p.material, p.density);
}
#endif

#ifndef SHARED_PRINT_VEC3
#define SHARED_PRINT_VEC3
void print_vec3(glm::vec3 vec)
{
    printf("(%f, %f, %f)", vec.x, vec.y, vec.z);
}
#endif

}