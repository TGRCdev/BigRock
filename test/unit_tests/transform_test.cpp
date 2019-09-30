#include <data/transform.hpp>
#include "../shared.hpp"
#include <glm/gtc/epsilon.hpp>
#include <stdio.h>

using namespace bigrock;

int main()
{
    printf("Transform Test\n");
    printf("Creating Transform with rotation angles (0, 0, 90)\n");
    Transform t;
    t.rotation = glm::quat(glm::radians(glm::vec3(0, 0, 90)));
    t.normalize();

    printf("Transforming local point (1,0,0) to global coordinates\n");
    glm::vec3 point(1,0,0);
    glm::vec3 expected(0,1,0);
    glm::vec3 result = t.to_global(point);
    float moe = 0.000001f;

    printf("Expected: "); print_vec3(expected); printf("\n");
    printf("Result: "); print_vec3(result); printf("\n");
    printf("Margin of error: %e\n", moe);
    
    bool passed = glm::all(glm::epsilonEqual(result, expected, moe));
    printf("Status: %s\n", passed ? "PASS" : "FAIL");
    if(!passed)
    {
        printf("Failed local-to-global transform test.\n");
        return 1;
    }

    printf("\nSetting scale to (0.5,0.5,0.5)\n");
    t.scale = glm::vec3(0.5,0.5,0.5);
    
    printf("Transforming local point (1,0,0) to global coordinates\n");
    expected.y = 0.5;
    result = t.to_global(point);

    printf("Expected: "); print_vec3(expected); printf("\n");
    printf("Result: "); print_vec3(result); printf("\n");
    printf("Margin of error: %e\n", moe);
    
    passed = glm::all(glm::epsilonEqual(result, expected, moe));
    printf("Status: %s\n", passed ? "PASS" : "FAIL");
    if(!passed)
    {
        printf("Failed local-to-global transform w/ scale test.\n");
        return 1;
    }

    printf("\nSetting origin to (2,1,3)\n");
    t.origin.x = 2;
    t.origin.y = 1;
    t.origin.z = 3;

    printf("Transforming local point (1,0,0) to global coordinates\n");
    expected.x = 2;
    expected.y = 1.5;
    expected.z = 3;
    result = t.to_global(point);

    printf("Expected: "); print_vec3(expected); printf("\n");
    printf("Result: "); print_vec3(result); printf("\n");
    printf("Margin of error: %e\n", moe);
    
    passed = glm::all(glm::epsilonEqual(result, expected, moe));
    printf("Status: %s\n", passed ? "PASS" : "FAIL");
    if(!passed)
    {
        printf("Failed local-to-global transform w/ scale & origin test.\n");
        return 1;
    }

    printf("\nTransforming global point "); print_vec3(result); printf(" to local coordinates\n");
    expected = glm::vec3(1,0,0);
    result = t.to_local(result);

    printf("Expected: "); print_vec3(expected); printf("\n");
    printf("Result: "); print_vec3(result); printf("\n");
    printf("Margin of error: %e\n", moe);
    
    passed = glm::all(glm::epsilonEqual(result, expected, 0.0001f));
    printf("Status: %s\n", passed ? "PASS" : "FAIL");
    if(!passed)
    {
        printf("Failed global-to-local transform test.\n");
        return 1;
    }
}