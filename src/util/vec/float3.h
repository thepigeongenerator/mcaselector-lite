#pragma once

#include <math.h>

#include "../atrb.h"

// stores a 3D point using a floating-point number
struct float3 {
	float x;
	float y;
	float z;
};
typedef struct float3 float3;

// adds one float3 to another
atrb_const static inline float3 float3_add(float3 v1, float3 v2) {
	return (float3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

// subtracts one float3 from another
atrb_const static inline float3 float3_sub(float3 v1, float3 v2) {
	return (float3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

// multiplies one float3 by another
atrb_const static inline float3 float3_mul(float3 v1, float3 v2) {
	return (float3){v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}

// divides one float3 by another
atrb_const static inline float3 float3_div(float3 v1, float3 v2) {
	return (float3){v1.x / v2.x, v1.y / v2.y, v1.z / v2.z};
}

// preforms a scalar multiplication upon the float3 (multiplies the float3 by some value)
atrb_const static inline float3 float3_mul_s(float3 v, float n) {
	return (float3){v.x * n, v.y * n, v.z * n};
}

// preforms a scalar division upon the float3 (divides the float3 by some value)
atrb_const static inline float3 float3_div_s(float3 v, float n) {
	return (float3){v.x / n, v.y / n, v.z / n};
}

atrb_const static inline float3 float3_neg(float3 v) {
	return (float3){-v.x, -v.y, -v.z};
}

// gets the squared magnitude/length of float3
atrb_const static inline float float3_mag2(float3 v) {
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

// gets the length of float3 (length)
atrb_const static inline float float3_mag(float3 v) {
	return sqrtf(float3_mag2(v));
}

// normalizes the float3
atrb_const static inline float3 float3_norm(float3 v) {
	float s = 1.0F / float3_mag(v); // get the scaling factor
	return float3_mul_s(v, s);      // scale the vector by the scaling factor (slightly more efficient than dividing)
}

// gets the dot product of two float3s
atrb_const static inline float float3_dot(float3 v1, float3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
