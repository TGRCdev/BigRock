#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 model;

void main()
{
    gl_Position = projection * camera * model * vec4(aPos, 1.0f);
}