#include "../src/data/cell.hpp"
#include "../src/data/tools/ellipsoid.hpp"
#include "../src/data/actions/emplace.hpp"
#include "../src/defines.hpp"

#include <time.h>
#include <iostream>
#include <string>

using namespace bigrock::data;
using namespace bigrock::data::tools;
using namespace bigrock::data::actions;
using namespace std;

int main()
{
    Cell cell;
    cout << "Tool Application Timing Test" << endl;
    Ellipsoid whole; // A tool taking up space in the entire cell
    whole.transform = Vector3(0.5f);
    Ellipsoid corner; // A tool only taking up the corner of the cell
    Ellipsoid small; // A tool only taking up a very small section of the corner of the cell
    small.transform.scale = Vector3(0.01f);
    Emplace e;
    printf("Subdivide Level|Whole time |Corner time|Small time |\n");
    printf("               |Whole size |Corner size|Small size |\n");
    printf("%s\n", string(52,'-').c_str());

    bool continue_whole = true, continue_corner = true, continue_small = true;
    #define TIME_THRESHOLD 1000 // How many milliseconds before testing stops
    #define CLOCKS_PER_MIL float(float(CLOCKS_PER_SEC)/1000)
    int subdiv_level = 1;
    while(subdiv_level < BR_MAX_CELL_DEPTH && (continue_whole || continue_corner || continue_small))
    {
        clock_t whole_start = 0, whole_end = 0, corner_start = 0, corner_end = 0, small_start = 0, small_end = 0;
        size_t whole_size = 0, corner_size = 0, small_size = 0;
        if(continue_whole)
        {
            whole_start = clock();
            cell.apply(whole, e, subdiv_level);
            whole_end = clock();
            if((whole_end - whole_start) > TIME_THRESHOLD)
                continue_whole = false;
            whole_size = cell.serialize()->length();
            cell.undivide();
        }
        if(continue_corner)
        {
            corner_start = clock();
            cell.apply(corner, e, subdiv_level);
            corner_end = clock();
            if((corner_end - corner_start) > TIME_THRESHOLD)
                continue_corner = false;
            corner_size = cell.serialize()->length();
            cell.undivide();
        }
        if(continue_small)
        {
            small_start = clock();
            cell.apply(small, e, subdiv_level);
            small_end = clock();
            if((small_end - small_start) > TIME_THRESHOLD)
                continue_small = false;
            small_size = cell.serialize()->length();
            cell.undivide();
        }
        printf("%-15d|", subdiv_level);
        if(continue_whole || whole_end != 0)
            printf("%-11s|", (std::to_string(float(whole_end - whole_start) / CLOCKS_PER_MIL).substr(0, 8) + " ms").c_str());
        else
            printf("%-11s|", "");
        
        if(continue_corner || corner_end != 0)
            printf("%-11s|", (std::to_string(float(corner_end - corner_start) / CLOCKS_PER_MIL).substr(0, 8) + " ms").c_str());
        else
            printf("%-11s|", "");
        
        if(continue_small || small_end != 0)
            printf("%-11s|", (std::to_string(float(small_end - small_start) / CLOCKS_PER_MIL).substr(0,8) + " ms").c_str());
        else
            printf("%-11s|", " ");
        
        printf("\n%-15s|", " ");

        if(whole_end != 0)
        {
            char type = 0; // 0 = bytes, 1 = KB, 2 = MB
            if(whole_size > 1e6)
                type = 2;
            else if(whole_size > 1000)
                type = 1;
            
            printf("%-11s|",
                type == 2 ? ("~" + to_string(float(whole_size) / 1e6 ).substr(0,7) + " MB").c_str() : 
                type == 1 ? ("~" + to_string(float(whole_size) / 1000).substr(0,7) + " KB").c_str() : 
                (to_string(whole_size) + " bytes").c_str()
            );
        }
        else
        {
            printf("%-11s|", "");
        }

        if(corner_end != 0)
        {
            char type = 0; // 0 = bytes, 1 = KB, 2 = MB
            if(corner_size > 1e6)
                type = 2;
            else if(corner_size > 1000)
                type = 1;
            
            printf("%-11s|",
                type == 2 ? ("~" + to_string(float(corner_size) / 1e6 ).substr(0,7) + " MB").c_str() : 
                type == 1 ? ("~" + to_string(float(corner_size) / 1000).substr(0,7) + " KB").c_str() : 
                (to_string(corner_size) + " bytes").c_str()
            );
        }
        else
        {
            printf("%-11s|", "");
        }

        if(small_end != 0)
        {
            char type = 0; // 0 = bytes, 1 = KB, 2 = MB
            if(small_size > 1e6)
                type = 2;
            else if(small_size > 1000)
                type = 1;
            
            printf("%-11s|",
                type == 2 ? ("~" + to_string(float(small_size) / 1e6 ).substr(0,7) + " MB").c_str() : 
                type == 1 ? ("~" + to_string(float(small_size) / 1000).substr(0,7) + " KB").c_str() : 
                (to_string(whole_size) + " bytes").c_str()
            );
        }
        else
        {
            printf("%-11s|", "");
        }
        printf("\n%s+\n", string(51,'-').c_str());

        subdiv_level++;
    }
}