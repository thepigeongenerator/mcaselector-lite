/* This file is part of MCA-Selector-lite,
 * and is licensed under GPL-2.0-only.
 * Copyright (C)2025 quinnthepigeon@proton.me Quinn
 * For further information, view COPYING and CONTRIBUTORS
 * at: www.github.com/thepigeongenerator/mcaselector-lite */
#version 330 core
layout(location = 0) in ivec4 rect; // rectangle data
uniform ivec2 screen;               // current screen dimensions

/* this vertex shader takes in unsigned integers formatted as (x,y,w,h), which denote rectangles
 * the shader is responsible to output the data for this rectangle in the same format,
 * but transformed to match the OpenGL coördinate space.
 * I found the formula `(x-0.5*l)/0.5*l` to work well for scaling an axis to fit within the -1—1 coördinate space.
 * And for transforming width/height, you just divide by the length of the corresponding axis.
 * I demonstrate this here: https://www.desmos.com/calculator/ptwuyiv6bh */
void main() {
	// divide the screen x and y coördinates by 2
	// using bit shift because I don't trust the compiler optimiser since I've got less control.
	int w_2 = screen.x >> 1;
	int h_2 = screen.y >> 1;

	gl_Position = vec4(
		(rect.x - w_2) / float(w_2),            // scale the rect X point
		(screen.y - rect.y - h_2) / float(h_2), // scale the rect Y point and invert the axis for top left alignment
		rect.z / float(w_2),                    // scale the rect width
		rect.w / float(h_2));                   // scale the rect height
}
