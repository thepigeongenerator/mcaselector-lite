/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#pragma once
#include <stdint.h>

#include "vec.h"

#define COLOUR32_BLACK   ((u8vec4){0x00, 0x00, 0x00, 0xFF})
#define COLOUR32_RED     ((u8vec4){0xFF, 0x00, 0x00, 0xFF})
#define COLOUR32_YELLOW  ((u8vec4){0xFF, 0xFF, 0x00, 0xFF})
#define COLOUR32_ORANGE  ((u8vec4){0xFF, 0x6D, 0x00, 0xFF})
#define COLOUR32_GREEN   ((u8vec4){0x00, 0xFF, 0x00, 0xFF})
#define COLOUR32_CYAN    ((u8vec4){0x00, 0xFF, 0xFF, 0xFF})
#define COLOUR32_BLUE    ((u8vec4){0x00, 0x00, 0xFF, 0xFF})
#define COLOUR32_MAGENTA ((u8vec4){0xFF, 0x00, 0xFF, 0xFF})
#define COLOUR32_WHITE   ((u8vec4){0xFF, 0xFF, 0xFF, 0xFF})

// american macros:
#define COLOR32_BLACK   COLOUR32_BLACK
#define COLOR32_RED     COLOUR32_RED
#define COLOR32_YELLOW  COLOUR32_YELLOW
#define COLOR32_ORANGE  COLOUR32_ORANGE
#define COLOR32_GREEN   COLOUR32_GREEN
#define COLOR32_CYAN    COLOUR32_CYAN
#define COLOR32_BLUE    COLOUR32_BLUE
#define COLOR32_MAGENTA COLOUR32_MAGENTA
#define COLOR32_WHITE   COLOUR32_WHITE
