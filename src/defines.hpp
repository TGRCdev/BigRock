#ifndef BIGROCK_DEFINES_H
#define BIGROCK_DEFINES_H
#pragma once

#include "flatbuffers/idl.h" // FLATBUFFERS_MAX_PARSING_DEPTH

#ifndef BR_MAX_CELL_DEPTH
#define BR_MAX_CELL_DEPTH 24
#endif

#ifndef BR_USE_DOUBLE_PRECISION
#define BR_USE_DOUBLE_PRECISION 0
#endif

#if BR_MAX_CELL_DEPTH > 24 && !BR_USE_DOUBLE_PRECISION
#pragma message("warning: It is recommended for cell depths higher than 24 to use double precision numbers. (BR_USE_DOUBLE_PRECISION)")
#endif

#if (BR_MAX_CELL_DEPTH + 2) > FLATBUFFERS_MAX_PARSING_DEPTH
#pragma message("warning: BR_MAX_CELL_DEPTH is greater than FLATBUFFERS_MAX_PARSING_DEPTH plus two. Cells with a higher depth will be truncated when serialized.")
#endif

#if BR_USE_DOUBLE_PRECISION
#define Vector3 glm::dvec3
#define Mat4 glm::dmat4
#define Quat glm::dquat
#define br_real_t double
#else
#define Vector3 glm::vec3
#define Mat4 glm::mat4
#define Quat glm::quat
#define br_real_t float
#endif

#endif