#include "../src/data/tools/ellipsoid.hpp"
#include "shared/common.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <time.h>
#include <sstream>

using namespace std;
using namespace bigrock::data;
using namespace tools;

int main()
{
    cout << "Tool Test" << endl;
    fstream ifile("ellipsoid.dat", ios::binary | ios::in);
    Ellipsoid tool;
    if(ifile.is_open())
    {
        stringstream sbuff;
        sbuff << ifile.rdbuf();
        string buff = sbuff.str();
        deserialize<Ellipsoid>(tool, buff.c_str());
        cout << "Input String: " << buff << endl;
        cout << "Input Tool: (" << tool.transform.origin.x << ", " << tool.transform.origin.y << ", " << tool.transform.origin.z << ")" << endl;
        ifile.close();
    }
    srand(time(NULL));
    rand();
    tool.transform.origin = glm::vec3(rand_range(-50, 50), rand_range(-50, 50), rand_range(-50, 50));
    //tool.transform.origin = glm::vec3(28.7011f);
    tool.transform.scale = glm::vec3(rand_range(-50, 50), rand_range(-50, 50), rand_range(-50, 50));
    cout << "Output Tool: (" << tool.transform.origin.x << ", " << tool.transform.origin.y << ", " << tool.transform.origin.z << ")" << endl;
    fstream file("ellipsoid.dat", ios::binary | ios::out);
    char buffer[50];
    int charcount = serialize(tool, buffer, true);
    if(charcount < 0)
    {
        cout << "Failed to serialize" << endl;
        return 1;
    }
    cout << "Output String: ";
    for(int i = 0; i < charcount; i++)
    {
        file << buffer[i];
        cout << buffer[i];
    }
    
    file.close();
    cout << endl;
}