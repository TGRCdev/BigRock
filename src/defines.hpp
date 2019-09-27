#ifndef BIGROCK_DEFINES_H
#define BIGROCK_DEFINES_H
#pragma once

#include "flatbuffers/idl.h" // FLATBUFFERS_MAX_PARSING_DEPTH
#include <cstdint>

#ifndef BR_MAX_CELL_DEPTH
#define BR_MAX_CELL_DEPTH 24
#endif

#if (BR_MAX_CELL_DEPTH + 2) > FLATBUFFERS_MAX_PARSING_DEPTH
#pragma message("warning: BR_MAX_CELL_DEPTH is greater than FLATBUFFERS_MAX_PARSING_DEPTH plus two. Cells with a higher depth will be truncated when serialized.")
#endif

namespace bigrock {

typedef float isovalue_t;
typedef unsigned char material_t;

typedef std::int8_t bitflags8_t;
typedef std::int16_t bitflags16_t;
typedef std::int32_t bitflags32_t;
typedef std::int64_t bitflags64_t;

}

#endif