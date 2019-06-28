#include "../src/data/cell.hpp"
#include "../src/data/actions/emplace.hpp"
#include "../src/data/tools/ellipsoid.hpp"
#include "../src/mesh/marchingcubescpu.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

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
#include "../src/defines.hpp"
#include "../src/data/transform.hpp"
#include <string>
#include <sstream>

#if !defined(_MSC_VER)
#include <unistd.h>
#endif

#define STARTWIDTH 800
#define STARTHEIGHT 600
#define MINWIDTH 640
#define MINHEIGHT 480

glm::mat4 projection;
Transform camera;

void window_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    projection = glm::perspective(70.0f, float(width) / height, 0.01f, 1000.0f);
}

#if defined(_WIN32)
#define sleep(seconds) Sleep(seconds * 1000)
#endif

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // Placeholder
}

static const char* glErrorString(GLenum err) {
  switch(err) {
    case GL_INVALID_ENUM: return "Invalid Enum";
    case GL_INVALID_VALUE: return "Invalid Value";
    case GL_INVALID_OPERATION: return "Invalid Operation";
    case GL_OUT_OF_MEMORY: return "Out of Memory";
    case GL_NO_ERROR: return "No Error";
    default: return "Unknown Error";
  }
}

typedef GLuint VBO;
typedef GLuint VAO;
typedef GLuint EBO;
typedef GLuint Program;
typedef GLuint Shader;

struct RenderModel
{
    VAO vao;
    unsigned int indices_size;
    Program program;
    Transform transform;
};

std::vector<RenderModel*> models;

#include "shaders/gen/basic.h"

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 camera_trns(camera);
    for(std::vector<RenderModel*>::const_iterator it = models.begin(); it != models.end(); it++)
    {
        RenderModel *model = (*it);
        if(!model || !model->vao || !model->program)
            continue;

        glBindVertexArray(model->vao);
        glUseProgram(model->program);
        
        GLint proj_loc = glGetUniformLocation(model->program, "projection");
        if(proj_loc != -1)
            glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection));
        GLint cam_loc = glGetUniformLocation(model->program, "camera");
        if(cam_loc != -1)
            glUniformMatrix4fv(cam_loc, 1, GL_FALSE, glm::value_ptr(camera_trns));
        GLint model_loc = glGetUniformLocation(model->program, "model");
        if(model_loc != -1)
        {
            glm::mat4 model_trns = glm::inverse(glm::mat4(model->transform));
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_trns));
        }
        
        glDrawElements(GL_TRIANGLES, model->indices_size, GL_UNSIGNED_INT, (void*)0);
    }
}

int main(int argc, char** argv)
{
    cout << "BigRock Test Program" << endl;
    int ok = glfwInit();
    if(!ok)
    {
        show_message("Failed to load OpenGL. Please update your computers drivers and confirm that it supports OpenGL 3.3.", "Failiure");
        return 1;
    }
    cout << "GLFW Initialized" << endl;

    // Setup window parameters
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the window
    GLFWwindow *window = glfwCreateWindow(STARTWIDTH, STARTHEIGHT, "BigRock Test Program", NULL, NULL);
    if(!window)
    {
        show_message("Failed to create a GLFW window context.", "Failiure");
        glfwTerminate();
        return 1;
    }
    cout << "GLFW Window created" << endl;
    glfwMakeContextCurrent(window);

    // Load GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        show_message("Failed to initialize GLAD.", "Failiure");
        glfwTerminate();
        return 1;
    }
    cout << "GLAD initialized" << endl;

    // Setup viewport
    glViewport(0, 0, STARTWIDTH, STARTHEIGHT);
    glfwSetFramebufferSizeCallback(window, window_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeLimits(window, MINWIDTH, MINHEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);

    // Test square
    RenderModel square;
    VBO tri_vbo;
    glGenBuffers(1, &tri_vbo);
    EBO tri_ebo;
    glGenBuffers(1, &tri_ebo);
    glGenVertexArrays(1, &square.vao);
    glBindVertexArray(square.vao);

    glBindBuffer(GL_ARRAY_BUFFER, tri_vbo);
    glm::vec3 tri_verts[] = {
        glm::vec3(0.5f, 0.5f, 0.0f),
        glm::vec3(0.5f, -0.5f, 0.0f),
        glm::vec3(-0.5f,-0.5f, 0.0f),
        glm::vec3(-0.5f,0.5f,0.0f)
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri_verts), tri_verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_ebo);
    unsigned int tri_index[] = {
        0, 1, 2,
        0, 2, 3
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tri_index), tri_index, GL_STATIC_DRAW);
    square.indices_size = 6;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    Program basic_prog = programs::basic::get_program();
    if(basic_prog == 0)
    {
        show_message((std::string("Shader program failed to link.\n\n") + std::string(programs::basic::get_error_message())).c_str());
        glfwTerminate();
        return 1;
    }
    square.program = basic_prog;

    models.push_back(&square);

    // Setup projection
    projection = glm::perspective(70.0f, float(STARTWIDTH) / STARTHEIGHT, 0.01f, 1000.0f);

    cout << "Beginning main loop" << endl;

    double totalTime = glfwGetTime();
    double lastTime = totalTime;

    glClearColor(0.1f, 0.3f, 0.4f, 1.0f);

    #define FRAMELIMIT 60
    const double frametime = double(1) / FRAMELIMIT;

    while(!glfwWindowShouldClose(window))
    {
        // Time
        double time = glfwGetTime();
        double delta = time - lastTime;
        totalTime += delta;
        lastTime = time;

        // Movement, input, etc.
        square.transform.rotation = glm::rotate(square.transform.rotation, float(delta), glm::vec3(0.0f,0.0f,1.0f));

        // Rendering
        render();

        // Error handling
        GLenum err = glGetError();
        if(err != GL_NO_ERROR)
        {
            stringstream err_stream;
            while(err != GL_NO_ERROR)
            {
                err_stream << glErrorString(err);
                err = glGetError();
                if(err != GL_NO_ERROR) err_stream << "\n";
            }
            show_message((std::string("An error occured while rendering the next frame.\n\n") + err_stream.str()).c_str());
            glfwTerminate();
            return 1;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Yield
        sleep(frametime);
    }

    glfwTerminate();
}