#include <data/ellipsoid.hpp>

#include "../shared.hpp"

using namespace bigrock;

int main()
{
    printf("\nEllipsoid Test\n");

    Ellipsoid e;
    printf("Created Ellipsoid\n");

    bool passed;
    float moe = 0.000001f;

    #define TEST_VALUE(value, expected, moe)\
    passed = glm::epsilonEqual(value, expected, moe);\
    printf("Expected: %f\n", expected);\
    printf("Actual: %f\n", value);\
    printf("Margin of Error: %e\n", moe);\
    printf("Status: %s\n\n", passed ? "PASS" : "FAIL");

    glm::vec3 p(0,0,0);
    printf("Retrieving value of "); print_vec3(p); printf("\n");
    TEST_VALUE(e.get_value(p), 1.0f, moe);
    if(!passed)
    {
        printf("Failed the origin value test.\n");
        return 1;
    }

    e.transform.origin = glm::vec3(3,0,0);
    printf("Setting Ellipsoid origin to "); print_vec3(e.transform.origin); printf("\n");
    TEST_VALUE(e.get_value(p), -1.0f, moe);
    if(!passed)
    {
        printf("Failed the translated value test.\n");
        return 1;
    }

    printf("Getting the local origin value of (0,0,0)\n");
    TEST_VALUE(e.get_value_local(glm::vec3(0,0,0)), 1.0f, moe);
    if(!passed)
    {
        printf("Failed the local origin value test.\n");
        return 1;
    }

    printf("Rotating tool by 90 degrees on the Z axis\n");
    e.transform.rotation = glm::rotate(e.transform.rotation, glm::radians(90.0f), glm::vec3(0,0,1));
    e.transform.normalize();

    p = e.transform.origin - glm::vec3(1,0,0);
    printf("Getting value at "); print_vec3(p); printf("\n");
    TEST_VALUE(e.get_value(p), 0.0f, moe);

    if(!passed)
    {
        printf("Failed the rotated value test.\n");
        return 1;
    }
}