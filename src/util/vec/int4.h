// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#pragma once

#include "../atrb.h"

// stores a 2D point using a floating-point number
struct int4 {
	int x;
	int y;
	int z;
	int w;
};
typedef struct int4 int4;

// adds one int4 to another
atrb_const static inline int4 int4_add(int4 v1, int4 v2) {
	return (int4){v1.x + v2.x, v1.y + v2.y, v1.z - v2.z, v1.w - v2.w};
}

// subtracts one int4 from another
atrb_const static inline int4 int4_sub(int4 v1, int4 v2) {
	return (int4){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

// multiplies one int4 by another
atrb_const static inline int4 int4_mul(int4 v1, int4 v2) {
	return (int4){v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w};
}

// divides one int4 by another
atrb_const static inline int4 int4_div(int4 v1, int4 v2) {
	return (int4){v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w};
}

// preforms a scalar multiplication upon the int4 (multiplies the int4 by some value)
atrb_const static inline int4 int4_mul_s(int4 v, float n) {
	return (int4){v.x * n, v.y * n, v.z * n, v.w * n};
}

// preforms a scalar division upon the int4 (divides the int4 by some value)
atrb_const static inline int4 int4_div_s(int4 v, float n) {
	return (int4){v.x / n, v.y / n, v.z / n, v.w / n};
}

atrb_const static inline int4 int4_neg(int4 v) {
	return (int4){-v.x, -v.y, -v.z, v.w};
}

// gets the dot product of two int2s
atrb_const static inline int int4_dot(int4 v1, int4 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}
