#pragma once

#include <math.h>

#include "../atrb.h"

// stores a 2D point using a floating-point number
struct float2 {
	float x;
	float y;
};
typedef struct float2 float2;

// adds one float2 to another
atrb_const static inline float2 float2_add(float2 v1, float2 v2) {
	return (float2){v1.x + v2.x, v1.y + v2.y};
}

// subtracts one float2 from another
atrb_const static inline float2 float2_sub(float2 v1, float2 v2) {
	return (float2){v1.x - v2.x, v1.y - v2.y};
}

// multiplies one float2 by another
atrb_const static inline float2 float2_mul(float2 v1, float2 v2) {
	return (float2){v1.x * v2.x, v1.y * v2.y};
}

// divides one float2 by another
atrb_const static inline float2 float2_div(float2 v1, float2 v2) {
	return (float2){v1.x / v2.x, v1.y / v2.y};
}

// preforms a scalar multiplication upon the float2 (multiplies the float2 by some value)
atrb_const static inline float2 float2_mul_s(float2 v, float n) {
	return (float2){v.x * n, v.y * n};
}

// preforms a scalar division upon the float2 (divides the float2 by some value)
atrb_const static inline float2 float2_div_s(float2 v, float n) {
	return (float2){v.x / n, v.y / n};
}

atrb_const static inline float2 float2_neg(float2 v) {
	return (float2){-v.x, -v.y};
}

// gets the squared magnitude/length of float2
atrb_const static inline float float2_mag2(float2 v) {
	return (v.x * v.x) + (v.y * v.y);
}

// gets the length of float2 (length)
atrb_const static inline float float2_mag(float2 v) {
	return sqrtf(float2_mag2(v));
}

// normalizes the float2
atrb_const static inline float2 float2_norm(float2 v) {
	float s = 1.0F / float2_mag(v); // get the scaling factor
	return float2_mul_s(v, s);      // scale the vector by the scaling factor (slightly more efficient than dividing)
}

// gets the dot product of two float2s
atrb_const static inline float float2_dot(float2 v1, float2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}
