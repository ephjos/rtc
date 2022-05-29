#pragma once

#include <stdbool.h>

#include "vec4.h"

//typedef float matrix_t[16];
typedef struct matrix_t {
	float _00; float _01; float _02; float _03;
	float _10; float _11; float _12; float _13;
	float _20; float _21; float _22; float _23;
	float _30; float _31; float _32; float _33;
} matrix_t;

matrix_t matrix(
		float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33
		);
matrix_t matrix_identity();

bool matrix_eq(matrix_t a, matrix_t b);

matrix_t matrix_mul(matrix_t a, matrix_t b);
vec4_t matrix_mul_vec4(matrix_t a, vec4_t b);
matrix_t matrix_transpose(matrix_t a);


