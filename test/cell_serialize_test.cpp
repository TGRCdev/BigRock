#include "../src/data/cell.hpp"
#include "shared/common.h"

//#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;
using namespace bigrock::data;

#define FILENAME "cell_data.bin"

void recursive_subdivide(Cell *cell, int max_depth)
{
    if(!cell || cell->get_depth() + 1 >= max_depth)
        return;
    
    if(cell->is_leaf())
        cell->subdivide();
    
    int rand_index = rand() % 8;
    
    recursive_subdivide(cell->get_child(rand_index), max_depth);
}

int main()
{
    srand(clock());
    rand();

    cout << "Cell Serialize Test" << endl;
    std::string str = load_binary_file_as_string(FILENAME);
    std::unique_ptr<Cell> cell;
    if(!str.empty())
    {
        cell = Cell::deserialize(str.c_str(), str.length());
        if(!cell)
        {
            cout << "Failed to load input Cell" << endl;
        }
        else
        {
            cout << "Input Cell Corners: [";
            for(int i = 0; i < 8; i++)
            {
                print_vec3(cell->get_corner_pos(i));
                if(i < 7)
                    cout << ", ";
            }
            cout << "]" << endl;
            cout << "Has Children: " << cell->has_children() << endl;
        }
    }
    cell.reset(new Cell(true));
    recursive_subdivide(cell.get(), BR_MAX_CELL_DEPTH);
    cout << "Output Cell Corners: [";
    for(int i = 0; i < 8; i++)
    {
        print_vec3(cell->get_corner_pos(i));
        if(i < 7)
            cout << ", ";
    }
    cout << "]" << endl;
    cout << "Has Children: " << cell->has_children() << endl;
    std::unique_ptr<std::string> outstr = cell->serialize();
    //cout << "Output String: " << output << endl;
    fstream ofile(FILENAME, ios::out | ios::binary);
    ofile << *outstr;
    ofile.close();
}