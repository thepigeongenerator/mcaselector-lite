// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#pragma once

#include "../atrb.h"

// stores a 2D point using a floating-point number
struct int2 {
	int x;
	int y;
};
typedef struct int2 int2;

// adds one int2 to another
atrb_const static inline int2 int2_add(int2 v1, int2 v2) {
	return (int2){v1.x + v2.x, v1.y + v2.y};
}

// subtracts one int2 from another
atrb_const static inline int2 int2_sub(int2 v1, int2 v2) {
	return (int2){v1.x - v2.x, v1.y - v2.y};
}

// multiplies one int2 by another
atrb_const static inline int2 int2_mul(int2 v1, int2 v2) {
	return (int2){v1.x * v2.x, v1.y * v2.y};
}

// divides one int2 by another
atrb_const static inline int2 int2_div(int2 v1, int2 v2) {
	return (int2){v1.x / v2.x, v1.y / v2.y};
}

// preforms a scalar multiplication upon the int2 (multiplies the int2 by some value)
atrb_const static inline int2 int2_mul_s(int2 v, float n) {
	return (int2){v.x * n, v.y * n};
}

// preforms a scalar division upon the int2 (divides the int2 by some value)
atrb_const static inline int2 int2_div_s(int2 v, float n) {
	return (int2){v.x / n, v.y / n};
}

atrb_const static inline int2 int2_neg(int2 v) {
	return (int2){-v.x, -v.y};
}

// gets the dot product of two int2s
atrb_const static inline int int2_dot(int2 v1, int2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}
