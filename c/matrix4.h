#pragma once

#include <stdbool.h>

#include "vec4.h"

//typedef float matrix4_t[16];
typedef struct matrix4_t {
	float _00; float _01; float _02; float _03;
	float _10; float _11; float _12; float _13;
	float _20; float _21; float _22; float _23;
	float _30; float _31; float _32; float _33;
} matrix4_t;

matrix4_t matrix4(
		float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33
		);
matrix4_t matrix4_identity();

bool matrix4_eq(matrix4_t a, matrix4_t b);

matrix4_t matrix4_mul(matrix4_t a, matrix4_t b);
vec4_t matrix4_mul_vec4(matrix4_t a, vec4_t b);
matrix4_t matrix4_transpose(matrix4_t a);
float matrix4_determinant(matrix4_t M);
bool matrix4_is_invertible(matrix4_t a);
matrix4_t matrix4_inverse(matrix4_t M);


