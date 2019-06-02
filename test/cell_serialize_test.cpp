#include "../src/data/cell.hpp"
#include "shared/common.h"
#include "../src/data/tools/ellipsoid.hpp"
#include "../src/data/actions/emplace.hpp"

//#include <iostream>
#include <fstream>
#include <time.h>

#include <glm/gtc/random.hpp>

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
    srand(time(0));
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
    cout << "Applying a 1.0 diameter sphere tool at (0.5, 0.5, 0.5) at subdivision level 5" << endl;
    tools::Ellipsoid t;
    t.transform.scale = Vector3(1.0f);
    t.transform.origin = Vector3(0.5f);
    actions::Emplace a;
    clock_t start = clock();
    cell->apply(t, a, 5);
    clock_t end = clock();
    cout << "Tool application took " << (end - start) << " clocks (about " << ((end - start) / (CLOCKS_PER_SEC/1000)) << " milliseconds)" << endl;

    cout << "Center sample: " << cell->sample(Vector3(0.5f,0.5f,0.5f)).isovalue << endl;
    cout << "Edge sample: " << cell->sample(Vector3(0.0f,0.5f,0.5f)).isovalue << endl;
    Vector3 rpoint = Vector3(rand_range(0.0f, 1.0f), rand_range(0.0f, 1.0f), rand_range(0.0f, 1.0f));
    cout << "Random sample at "; print_vec3(rpoint); cout << ": " << cell->sample(rpoint).isovalue << endl;
    std::unique_ptr<std::string> outstr = cell->serialize();
    if(!outstr)
    {
        cout << "Failed to serialize cell" << endl;
        return 1;
    }
    cout << "Output file size: ~";
    size_t filesize = outstr->length();
    if(filesize >= 1e+6)
        cout << (float(filesize) / 1e+6) << " MB";
    else if(filesize > 1000)
        cout << (float(filesize) / 1000) << " KB";
    else
        cout << filesize << " bytes";
    cout << endl;
    fstream ofile(FILENAME, ios::out | ios::binary);
    ofile << *outstr;
    ofile.close();
    return 0;
}