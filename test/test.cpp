#include "../src/data/cell.hpp"
#include "../src/data/actions/emplace.hpp"
#include "../src/data/tools/ellipsoid.hpp"
#include "../src/mesh/marchingcubescpu.hpp"

#include <iostream>
#include <string>

#if defined(_WIN32)
#include <windows.h>
#include <WinUser.h>
#endif

using namespace std;
using namespace bigrock::data;
using namespace bigrock::mesh;

void show_message(const char *message, const char *title = NULL)
{
    #if defined(_WIN32)
    MessageBox(NULL, reinterpret_cast<LPCTSTR>(message), reinterpret_cast<LPCTSTR>(title), MB_OK);
    #else
    cout << message << endl;
    #endif
}

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(int argc, char** argv)
{
    int ok = glfwInit();
    if(!ok)
    {
        show_message("Failed to load OpenGL. Please update your computers drivers and confirm that it supports OpenGL 3.3.");
        return 1;
    }
    else
    {
        show_message("OpenGL loaded successfully.", "Success");
        return 0;
    }
}