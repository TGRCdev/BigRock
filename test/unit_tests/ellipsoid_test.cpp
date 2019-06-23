#include "../../src/data/tools/ellipsoid.hpp"
#include "../shared/common.h"

#include <glm/vec3.hpp>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <iomanip>

using namespace std;
using namespace bigrock::data::tools;

int main()
{
    Ellipsoid tool;
    cout << "Ellipsoid Tool Value Test" << endl;

    srand(time(NULL));
    rand();
    unsigned int vec_count = 3 + (rand() % 13);

    cout << "index|" << setw(33) << "Vector|" << setw(8) << "Length|" << setw(9) << "Value" << endl;

    for(int i = 0; i < vec_count; i++)
    {
        glm::vec3 vec = glm::vec3(rand_range(-0.5, 0.5), rand_range(-0.5, 0.5), rand_range(-0.5, 0.5));
        cout << setw(5) << i << "| "; print_vec3(vec); cout << " |" << setw(8) << glm::length(vec) << "|" << setw(9) << tool.value(vec) << endl;
    }
}