#ifndef BIGROCK_GRID_VERTICES_H
#define BIGROCK_GRID_VERTICES_H
#pragma once

#include <glm/vec3.hpp>

namespace bigrock {

const glm::vec3 GRID_VERTICES[8] = {
    glm::vec3(0.0f,0.0f,0.0f),
    glm::vec3(1.0f,0.0f,0.0f),
    glm::vec3(0.0f,1.0f,0.0f),
    glm::vec3(1.0f,1.0f,0.0f),
    glm::vec3(0.0f,0.0f,1.0f),
    glm::vec3(1.0f,0.0f,1.0f),
    glm::vec3(0.0f,1.0f,1.0f),
    glm::vec3(1.0f,1.0f,1.0f)
};

}

#endif