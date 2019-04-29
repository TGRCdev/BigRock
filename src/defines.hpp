#ifndef BIGROCK_DEFINES_H
#define BIGROCK_DEFINES_H
#pragma once

#ifndef BR_DEFAULT_MAX_SUBDIV
#define BR_DEFAULT_MAX_SUBDIV 4
#endif

#pragma region Pooling Defines

#ifndef BR_USE_POOLING
#define BR_USE_POOLING 1
#endif

#ifndef BR_EXPAND_POOL
#define BR_EXPAND_POOL 0
#endif

#ifndef BR_DEFAULT_POOL_SIZE
#define BR_DEFAULT_POOL_SIZE 256
#endif

#ifndef BR_DEFAULT_POOL_EXPAND_SIZE
#define BR_DEFAULT_POOL_EXPAND_SIZE 32
#endif

#ifndef BR_CELL_POOL_SIZE
#define BR_CELL_POOL_SIZE 256
#endif

#ifndef BR_POINT_POOL_SIZE
#define BR_POINT_POOL_SIZE 1024
#endif

#pragma endregion

#endif