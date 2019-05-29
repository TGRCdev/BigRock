#include "../src/data/cell.hpp"
#include "shared/common.h"

//#include <iostream>
#include <fstream>

using namespace std;
using namespace bigrock::data;

#define FILENAME "cell_data.bin"

void recursive_subdivide(Cell *cell, int max_depth)
{
    if(cell->get_depth() >= max_depth)
        return;
    
    if(cell->is_leaf())
        cell->subdivide();
    
    recursive_subdivide(cell->get_child(0), max_depth);
}

int main()
{
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
                print_vec3(cell->get_corner(i).position);
                if(i < 7)
                    cout << ", ";
            }
            cout << "]" << endl;
            cout << "Has Children: " << cell->has_children() << endl;
        }
    }
    cell.reset(new Cell(true));
    recursive_subdivide(cell.get(), 8);
    cout << "Output Cell Corners: [";
    for(int i = 0; i < 8; i++)
    {
        print_vec3(cell->get_corner(i).position);
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