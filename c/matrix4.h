#pragma once

#include <stdbool.h>

#include "vec4.h"

//typedef double matrix4_t[16];
typedef struct matrix4_t {
	double _00; double _01; double _02; double _03;
	double _10; double _11; double _12; double _13;
	double _20; double _21; double _22; double _23;
	double _30; double _31; double _32; double _33;
} matrix4_t;

matrix4_t matrix4(
		double _00, double _01, double _02, double _03,
		double _10, double _11, double _12, double _13,
		double _20, double _21, double _22, double _23,
		double _30, double _31, double _32, double _33
		);
matrix4_t matrix4_identity();

bool matrix4_eq(matrix4_t a, matrix4_t b);

matrix4_t matrix4_mul(matrix4_t a, matrix4_t b);
vec4_t matrix4_mul_vec4(matrix4_t a, vec4_t b);
matrix4_t matrix4_transpose(matrix4_t a);
double matrix4_determinant(matrix4_t M);
bool matrix4_is_invertible(matrix4_t a);
matrix4_t matrix4_inverse(matrix4_t M);


