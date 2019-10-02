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

#define RAND_ISOVALUE() ((float(rand()) / RAND_MAX) * 2.0f) - 1.0f
#define RAND_MATERIAL() material_t(rand())
#define BOOL2STR(val) (val ? "true" : "false")

}