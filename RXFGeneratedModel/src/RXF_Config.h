/* This file is auto generated from Rhapsody do not edit! */

#ifndef RXF_Config_H
#define RXF_Config_H

#include "RXF_Defines.h"

#define RXF_REACTIVE_MAX_NULL_STEPS 10U
#define RXF_MAX_TIMEOUTS 32U
#define RXF_MS_PER_TICK 10U

#define RXF_TIMEOUT_GUARANTEE_TIME

#define RXF_HIGH_WATER_MARKS_ENABLED
#undef RXF_HIGH_WATER_MARKS_ENABLED

#define RXF_MEMORY_TINY_BLOCK_COUNT 0U
#define RXF_MEMORY_TINY_BLOCK_SIZE 0U
#define RXF_MEMORY_SMALL_BLOCK_COUNT 32U
#define RXF_MEMORY_SMALL_BLOCK_SIZE 16U
#define RXF_MEMORY_MEDIUM_BLOCK_COUNT 24U
#define RXF_MEMORY_MEDIUM_BLOCK_SIZE 32U
#define RXF_MEMORY_LARGE_BLOCK_COUNT 32U
#define RXF_MEMORY_LARGE_BLOCK_SIZE 128U
#define RXF_MEMORY_HUGE_BLOCK_COUNT 0U
#define RXF_MEMORY_HUGE_BLOCK_SIZE 0U

#define RXF_MEMORY_MAX_BLOCK_SIZE RXF_MEMORY_LARGE_BLOCK_SIZE

#define RXF_VTBL_HANDLE_EVENT_NOT_CONSUMED

#define RXF_MEMORY_FORCE_ALLOC_TOO_LARGE

#endif /* RXF_Config_H */
