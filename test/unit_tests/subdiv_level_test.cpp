#include "../../src/data/cell.hpp"
#include "../../src/data/tools/ellipsoid.hpp"
#include "../../src/data/actions/emplace.hpp"
#include "../../src/defines.hpp"
#include "../../src/data/jobpool.hpp"

#include <time.h>
#include <iostream>
#include <string>

using namespace bigrock::data;
using namespace bigrock::data::tools;
using namespace bigrock::data::actions;
using namespace std;

#if BR_DISABLE_MULTITHREADING
#define USE_MULTI false
#else
#define USE_MULTI true
#endif

int main()
{
    #undef small
    Cell cell;
    cout << "Tool Application Timing Test" << endl;
    cout << "Number of cores: " << JobPool::get_number_of_cores() << endl;
    Ellipsoid whole; // A tool taking up space in the entire cell
    whole.transform = glm::vec3(0.5f);
    Ellipsoid corner; // A tool only taking up the corner of the cell
    Ellipsoid small; // A tool only taking up a very small section of the corner of the cell
    small.transform.scale = glm::vec3(0.01f);
    Emplace e;
    printf("Subdiv|Whole time |Corner time|Small time |\n");
    printf("%s+\n", string(42,'-').c_str());

    bool continue_whole = true, continue_corner = true, continue_small = true;
    #define TIME_THRESHOLD 1.0 // How long subdivision should take before the test ends
    int subdiv_level = 1;
    while(subdiv_level < BR_MAX_CELL_DEPTH && (continue_whole || continue_corner || continue_small))
    {
        clock_t whole_start = 0, whole_end = 0, corner_start = 0, corner_end = 0, small_start = 0, small_end = 0;
        if(continue_whole)
        {
            whole_start = clock();
            cell.apply(whole, e, subdiv_level, USE_MULTI);
            whole_end = clock();
            if(float(whole_end - whole_start) / CLOCKS_PER_SEC > TIME_THRESHOLD)
                continue_whole = false;
            cell.~Cell();
            new (&cell) (Cell) ();
        }
        if(continue_corner)
        {
            corner_start = clock();
            cell.apply(corner, e, subdiv_level, USE_MULTI);
            corner_end = clock();
            if(float(corner_end - corner_start) / CLOCKS_PER_SEC > TIME_THRESHOLD)
                continue_corner = false;
            cell.~Cell();
            new (&cell) (Cell) ();
        }
        if(continue_small)
        {
            small_start = clock();
            cell.apply(small, e, subdiv_level, USE_MULTI);
            small_end = clock();
            if(float(small_end - small_start) / CLOCKS_PER_SEC > TIME_THRESHOLD)
                continue_small = false;
            cell.~Cell();
            new (&cell) (Cell) ();
        }
        printf("%-6d|", subdiv_level);
        if(continue_whole || whole_end != 0)
            printf("%-11s|", (std::to_string(float(whole_end - whole_start) / CLOCKS_PER_SEC).substr(0, 8) + " s").c_str());
        else
            printf("%-11s|", "");
        
        if(continue_corner || corner_end != 0)
            printf("%-11s|", (std::to_string(float(corner_end - corner_start) / CLOCKS_PER_SEC).substr(0, 8) + " s").c_str());
        else
            printf("%-11s|", "");
        
        if(continue_small || small_end != 0)
            printf("%-11s|", (std::to_string(float(small_end - small_start) / CLOCKS_PER_SEC).substr(0,8) + " s").c_str());
        else
            printf("%-11s|", " ");
        printf("\n%s+\n", string(42,'-').c_str());
        subdiv_level++;
    }
    cout << "Subdivision level test complete." << endl;
}