#include <data/point.hpp>
#include "../shared.hpp"

#include <cmath>
#include <time.h>
#include <assert.h>
#include <stdio.h>

using namespace bigrock;

#define RAND_ISOVALUE() ((float(rand()) / RAND_MAX) * 2.0f) - 1.0f
#define RAND_MATERIAL() material_t(rand())
#define BOOL2STR(val) (val ? "true" : "false")

#define TEST_RUNS 15

bool collapse_transitive_test(unsigned short test_runs)
{
    if(test_runs == 0)
        return true;
    
    printf("Performing Transitive Collapse Test\nNumber of runs: %u\n", test_runs);
    Point p1;
    Point p2;
    Point p3;

    printf("-------------------------------------------------\n");
    printf("     |        |        |p1 => p3|p1 => p3|      |\n");
    printf("Run #|p1 => p2|p2 => p3|Expected| Actual |Status|\n");
    printf("-------------------------------------------------\n");

    bool all_pass = true;
    for(unsigned short i = 0; i < test_runs; i++)
    {
        p1.density = RAND_ISOVALUE();
        p2.density = RAND_ISOVALUE();
        p3.density = RAND_ISOVALUE();
        p1.material = RAND_MATERIAL();
        p2.material = RAND_MATERIAL();
        p3.material = RAND_MATERIAL();
        bool collapses[3] = {
            p1.can_collapse(p2),
            p2.can_collapse(p3),
            p1.can_collapse(p3)
        };
        char expected_value;
        if(collapses[0])
        {
            if(collapses[1])
                expected_value = 1;
            else
                expected_value = 0;
        }
        else
        {
            if(collapses[1])
                expected_value = 0;
            else
                expected_value = -1;
        }
        bool passed = (expected_value == -1 || (bool(expected_value) == collapses[2]));
        printf("%-5u|%-8s|%-8s|%-8s|%-8s|%-6s|\n", 
            i+1,
            BOOL2STR(collapses[0]),
            BOOL2STR(collapses[1]),
            (expected_value == -1 ? "any" : BOOL2STR(bool(expected_value))),
            BOOL2STR(collapses[2]),
            passed ? "PASS" : "FAIL"
        );
        if(!passed)
            all_pass = false;
    }
    printf("-------------------------------------------------\n");
    return all_pass;
}

bool collapse_symmetric_test(unsigned short test_runs)
{
    if(test_runs == 0)
        return true;
    
    printf("Performing Symmetric Collapse Test\nNumber of runs: %u\n", test_runs);
    Point p1;
    Point p2;

    printf("-------------------------------\n");
    printf("Run #|p1 => p2|p2 => p1|Status|\n");
    printf("-------------------------------\n");

    bool all_pass = true;
    for(unsigned short i = 0; i < test_runs; i++)
    {
        p1.density = RAND_ISOVALUE();
        p2.density = RAND_ISOVALUE();
        p1.material = RAND_MATERIAL();
        p2.material = RAND_MATERIAL();

        bool collapses[2] = {
            p1.can_collapse(p2),
            p2.can_collapse(p1)
        };

        bool passes = (collapses[0] == collapses[1]);
        printf("%-5u|%-8s|%-8s|%-6s|\n",
            i,
            BOOL2STR(collapses[0]),
            BOOL2STR(collapses[1]),
            passes ? "PASS" : "FAIL"
        );
        if(!passes)
            all_pass = false;
    }
    printf("-------------------------------\n");
    return all_pass;
}

bool collapse_reflexive_test(unsigned short test_runs)
{
    if(test_runs == 0)
        return true;
    
    printf("Performing Reflexive Collapse Test\nNumber of runs: %u\n", test_runs);
    Point p1;

    printf("----------------------\n");
    printf("Run #|p1 => p1|Status|\n");
    printf("----------------------\n");

    bool all_pass = true;
    for(unsigned short i = 0; i < test_runs; i++)
    {
        p1.density = RAND_ISOVALUE();
        p1.material = RAND_MATERIAL();
        bool collapses = p1.can_collapse(p1);
        printf("%-5u|%-8s|%-6s|\n",
            i,
            BOOL2STR(collapses),
            collapses ? "PASS" : "FAIL"
        );
        if(!collapses)
            all_pass = false;
    }
    printf("----------------------\n");
    return all_pass;
}

int main()
{
    srand(time(NULL));
    rand();
    printf("BigRock Point Class Test\n\n");
    assert(collapse_reflexive_test(TEST_RUNS));
    printf("\n");
    assert(collapse_symmetric_test(TEST_RUNS));
    printf("\n");
    assert(collapse_transitive_test(TEST_RUNS));
    printf("\n");
    printf("Static Tests\n");
    Point p1(2, 0.37);
    Point p2(1, 0.79);
    printf("Point 1: "); print_point(p1); printf("\n");
    printf("Point 2: "); print_point(p2); printf("\n");
    printf("Interp Amount: 0.5\n");
    Point p3 = p1.lerp(p2, 0.5);
    printf("Result: "); print_point(p3); printf("\n");
    printf("Expected Material: %u\n", p2.material);
    printf("Actual Material: %u\n", p3.material);
    printf("Status: %s\n", p2.material == p3.material ? "PASS" : "FAIL");
    assert(p2.material == p3.material);
}