// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#pragma once
#include <stdint.h>

// stores colour in a rgba format stored as little-endian, each channel being a 8 bits wide.
typedef union colour32 {
	uint32_t dat; // full colour data; little-endian
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	// colour channel information for little-endian systems
	struct {
		uint8_t a, b, g, r;
	} ch;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	// colour channel information for big-endian systems
	struct {
		uint8_t r, g, b, a;
	} ch;
#endif
} colour32;

#define COLOUR32_BLACK   ((colour32){0x000000FF})
#define COLOUR32_RED     ((colour32){0xFF0000FF})
#define COLOUR32_YELLOW  ((colour32){0xFFFF00FF})
#define COLOUR32_ORANGE  ((colour32){0xFF6D00FF})
#define COLOUR32_GREEN   ((colour32){0x00FF00FF})
#define COLOUR32_CYAN    ((colour32){0x00FFFFFF})
#define COLOUR32_BLUE    ((colour32){0x0000FFFF})
#define COLOUR32_MAGENTA ((colour32){0xFF00FFFF})
#define COLOUR32_WHITE   ((colour32){0xFFFFFFFF})

// american macros:
#define color32         colour32
#define COLOR32_BLACK   COLOUR32_BLACK
#define COLOR32_RED     COLOUR32_RED
#define COLOR32_YELLOW  COLOUR32_YELLOW
#define COLOR32_ORANGE  COLOUR32_ORANGE
#define COLOR32_GREEN   COLOUR32_GREEN
#define COLOR32_CYAN    COLOUR32_CYAN
#define COLOR32_BLUE    COLOUR32_BLUE
#define COLOR32_MAGENTA COLOUR32_MAGENTA
#define COLOR32_WHITE   COLOUR32_WHITE
