#include "../src/data/cell.hpp"

#include <iostream>

using namespace std;
using namespace bigrock::data;

int main()
{
    cout << "Cell Test" << endl;
    cout << "Cell Size: " << sizeof(Cell) << " bytes" << endl;
    cout << "Creating a cell...";
    Cell cell;
    cout << "Done." << endl;
    cout << "cell.is_leaf() = " << cell.is_leaf() << endl;
    cout << "Subdividing cell...";
    cell.subdivide();
    cout << "Done." << endl;
    cout << "cell.is_leaf() = " << cell.is_leaf() << endl;
    cout << "Undividing cell...";
    cell.undivide();
    cout << "Done." << endl;
    cout << "cell.is_leaf() = " << cell.is_leaf() << endl;
    return 0;
}