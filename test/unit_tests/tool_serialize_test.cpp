#include "../../src/data/tools/ellipsoid.hpp"
#include "../shared/common.h"
#include "../../src/defines.hpp"

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
    cout << "Tool Serialize Test" << endl;
    fstream ifile("ellipsoid.dat", ios::binary | ios::in);
    std::unique_ptr<Tool> tool;
    if(ifile.is_open())
    {
        stringstream sbuff;
        sbuff << ifile.rdbuf();
        string buff = sbuff.str();
        //deserialize<Ellipsoid>(tool, buff.c_str());
        tool = Tool::deserialize(buff.c_str(), buff.length());
        cout << "Input String: " << buff << endl;
        if(tool)
        {
            cout << "Input Tool: (" << tool->transform.origin.x << ", " << tool->transform.origin.y << ", " << tool->transform.origin.z << ")" << endl;
        }
        else
        {
            cout << "Failed to load tool from file" << endl;
        }
        ifile.close();
    }
    tool.reset(new Ellipsoid());
    srand(time(NULL));
    rand();
    tool->transform.origin = Vector3(rand_range(-50, 50), rand_range(-50, 50), rand_range(-50, 50));
    //tool.transform.origin = Vector3(28.7011f);
    tool->transform.scale = Vector3(rand_range(-50, 50), rand_range(-50, 50), rand_range(-50, 50));
    cout << "Output Tool: (" << tool->transform.origin.x << ", " << tool->transform.origin.y << ", " << tool->transform.origin.z << ")" << endl;
    fstream file("ellipsoid.dat", ios::binary | ios::out);
    std::string buffer = tool->serialize();
    if(buffer.empty())
    {
        cout << "Failed to serialize" << endl;
        return 1;
    }
    cout << "Output String: ";
    cout << buffer << endl;
    cout << "Output String Size: " << buffer.length() << endl;
    cout << "Tool Size: " << sizeof(Ellipsoid) << endl;
    
    file.write(&buffer[0], buffer.length());
    file.close();
    return 0;
}