#ifndef BIGROCK_CUBE_VERTICES_H
#define BIGROCK_CUBE_VERTICES_H
#pragma once

#include <glm/vec3.hpp>

namespace bigrock {

/// Cube vertices are defined in binary order, which means that each bit of the index
/// represents an axis, and the value of said bit determines whether it is shifted along
/// that axis or not. Bit 1 is the X axis, bit 2 is the Y axis, and bit 3 is the Z axis.
///
/// For example, index 5 (101) would be shifted along the X and Z axis.
const glm::vec3 CUBE_VERTICES[8] = {
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