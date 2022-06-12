
#include "labrat.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "error.h"
#include "matrix4.h"
#include "util.h"
#include "vec4.h"

matrix4_t matrix4(
		float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33
		) {
	matrix4_t m;
	m._00 = _00; m._01 = _01; m._02 = _02; m._03 = _03;
	m._10 = _10; m._11 = _11; m._12 = _12; m._13 = _13;
	m._20 = _20; m._21 = _21; m._22 = _22; m._23 = _23;
	m._30 = _30; m._31 = _31; m._32 = _32; m._33 = _33;
	return m;
}

matrix4_t matrix4_identity() {
	matrix4_t m;
	m._00 = 1; m._01 = 0; m._02 = 0; m._03 = 0;
	m._10 = 0; m._11 = 1; m._12 = 0; m._13 = 0;
	m._20 = 0; m._21 = 0; m._22 = 1; m._23 = 0;
	m._30 = 0; m._31 = 0; m._32 = 0; m._33 = 1;
	return m;
}

bool matrix4_eq(matrix4_t a, matrix4_t b) {
	return req(a._00, b._00) &&
		req(a._01, b._01) &&
		req(a._02, b._02) &&
		req(a._03, b._03) &&
		req(a._10, b._10) &&
		req(a._11, b._11) &&
		req(a._12, b._12) &&
		req(a._13, b._13) &&
		req(a._20, b._20) &&
		req(a._21, b._21) &&
		req(a._22, b._22) &&
		req(a._23, b._23) &&
		req(a._30, b._30) &&
		req(a._31, b._31) &&
		req(a._32, b._32) &&
		req(a._33, b._33);
}

matrix4_t matrix4_mul(matrix4_t a, matrix4_t b) {
	matrix4_t m;
	m._00 = (a._00 * b._00) + (a._01 * b._10) + (a._02 * b._20) + (a._03 * b._30);
	m._01 = (a._00 * b._01) + (a._01 * b._11) + (a._02 * b._21) + (a._03 * b._31);
	m._02 = (a._00 * b._02) + (a._01 * b._12) + (a._02 * b._22) + (a._03 * b._32);
	m._03 = (a._00 * b._03) + (a._01 * b._13) + (a._02 * b._23) + (a._03 * b._33);

	m._10 = (a._10 * b._00) + (a._11 * b._10) + (a._12 * b._20) + (a._13 * b._30);
	m._11 = (a._10 * b._01) + (a._11 * b._11) + (a._12 * b._21) + (a._13 * b._31);
	m._12 = (a._10 * b._02) + (a._11 * b._12) + (a._12 * b._22) + (a._13 * b._32);
	m._13 = (a._10 * b._03) + (a._11 * b._13) + (a._12 * b._23) + (a._13 * b._33);

	m._20 = (a._20 * b._00) + (a._21 * b._10) + (a._22 * b._20) + (a._23 * b._30);
	m._21 = (a._20 * b._01) + (a._21 * b._11) + (a._22 * b._21) + (a._23 * b._31);
	m._22 = (a._20 * b._02) + (a._21 * b._12) + (a._22 * b._22) + (a._23 * b._32);
	m._23 = (a._20 * b._03) + (a._21 * b._13) + (a._22 * b._23) + (a._23 * b._33);

	m._30 = (a._30 * b._00) + (a._31 * b._10) + (a._32 * b._20) + (a._33 * b._30);
	m._31 = (a._30 * b._01) + (a._31 * b._11) + (a._32 * b._21) + (a._33 * b._31);
	m._32 = (a._30 * b._02) + (a._31 * b._12) + (a._32 * b._22) + (a._33 * b._32);
	m._33 = (a._30 * b._03) + (a._31 * b._13) + (a._32 * b._23) + (a._33 * b._33);
	return m;
}

vec4_t matrix4_mul_vec4(matrix4_t a, vec4_t b) {
	vec4_t v;
	v.x = (a._00 * b.x) + (a._01 * b.y) + (a._02 * b.z) + (a._03 * b.w);
	v.y = (a._10 * b.x) + (a._11 * b.y) + (a._12 * b.z) + (a._13 * b.w);
	v.z = (a._20 * b.x) + (a._21 * b.y) + (a._22 * b.z) + (a._23 * b.w);
	v.w = (a._30 * b.x) + (a._31 * b.y) + (a._32 * b.z) + (a._33 * b.w);
	return v;
}

matrix4_t matrix4_transpose(matrix4_t a) {
	matrix4_t m;
	m._00 = a._00; m._10 = a._01; m._20 = a._02; m._30 = a._03;
	m._01 = a._10; m._11 = a._11; m._21 = a._12; m._31 = a._13;
	m._02 = a._20; m._12 = a._21; m._22 = a._22; m._32 = a._23;
	m._03 = a._30; m._13 = a._31; m._23 = a._32; m._33 = a._33;
	return m;
}

float matrix4_determinant(matrix4_t M) {
	float a = M._00;
	float b = M._01;
	float c = M._02;
	float d = M._03;
	float e = M._10;
	float f = M._11;
	float g = M._12;
	float h = M._13;
	float i = M._20;
	float j = M._21;
	float k = M._22;
	float l = M._23;
	float m = M._30;
	float n = M._31;
	float o = M._32;
	float p = M._33;

	float ca = f * (k * p - l * o) - g * (j * p - l * n) + h * (j * o - k * n);
	float cb = -e * (k * p - l * o) + g * (i * p - l * m) - h * (i * o - k * m);
	float cc = e * (j * p - l * n) - f * (i * p - l * m) + h * (i * n - j * m);
	float cd = -e * (j * o - k * n) + f * (i * o - k * m) - g * (i * n - j * m);

	return a * ca + b * cb + c * cc + d * cd;
}

bool matrix4_is_invertible(matrix4_t a) {
	return matrix4_determinant(a) != 0;
}

matrix4_t matrix4_inverse(matrix4_t M) {
	float a = M._00;
	float b = M._01;
	float c = M._02;
	float d = M._03;
	float e = M._10;
	float f = M._11;
	float g = M._12;
	float h = M._13;
	float i = M._20;
	float j = M._21;
	float k = M._22;
	float l = M._23;
	float m = M._30;
	float n = M._31;
	float o = M._32;
	float p = M._33;

	float ca = f * (k * p - l * o) - g * (j * p - l * n) + h * (j * o - k * n);
	float cb = -e * (k * p - l * o) + g * (i * p - l * m) - h * (i * o - k * m);
	float cc = e * (j * p - l * n) - f * (i * p - l * m) + h * (i * n - j * m);
	float cd = -e * (j * o - k * n) + f * (i * o - k * m) - g * (i * n - j * m);

	float det = a * ca + b * cb + c * cc + d * cd;
	if (det == 0) {
		fprintf(stderr, "Matrix is not invertible\n");
		exit(ERROR_MATRIX_NONINVERTIBLE);
	}

	float ce = -b * (k * p - l * o) + c * (j * p - l * n) - d * (j * o - k * n);
	float cf = a * (k * p - l * o) - c * (i * p - l * m) + d * (i * o - k * m);
	float cg = -a * (j * p - l * n) + b * (i * p - l * m) - d * (i * n - j * m);
	float ch = a * (j * o - k * n) - b * (i * o - k * m) + c * (i * n - j * m);

	float ci = b * (g * p - h * o) - c * (f * p - h * n) + d * (f * o - g * n);
	float cj = -a * (g * p - h * o) + c * (e * p - h * m) - d * (e * o - g * m);
	float ck = a * (f * p - h * n) - b * (e * p - h * m) + d * (e * n - f * m);
	float cl = -a * (f * o - g * n) + b * (e * o - g * m) - c * (e * n - f * m);

	float cm = -b * (g * l - h * k) + c * (f * l - h * j) - d * (f * k - g * j);
	float cn = a * (g * l - h * k) - c * (e * l - h * i) + d * (e * k - g * i);
	float co = -a * (f * l - h * j) + b * (e * l - h * i) - d * (e * j - f * i);
	float cp = a * (f * k - g * j) - b * (e * k - g * i) + c * (e * j - f * i);

	return matrix4(
			ca / det,
			ce / det,
			ci / det,
			cm / det,
			cb / det,
			cf / det,
			cj / det,
			cn / det,
			cc / det,
			cg / det,
			ck / det,
			co / det,
			cd / det,
			ch / det,
			cl / det,
			cp / det
			);
}

TEST_CASE(construct_4x4_matrix4) {
	matrix4_t m = matrix4(
			1, 2, 3, 4,
			5.5, 6.5, 7.5, 8.5,
			9, 10, 11, 12,
			13.5, 14.5, 15.5, 16.5
			);

	ASSERT_EQ(m._00, 1.0, "%f");
	ASSERT_EQ(m._03, 4.0, "%f");
	ASSERT_EQ(m._10, 5.5, "%f");
	ASSERT_EQ(m._12, 7.5, "%f");
	ASSERT_EQ(m._22, 11.0, "%f");
	ASSERT_EQ(m._30, 13.5, "%f");
	ASSERT_EQ(m._32, 15.5, "%f");
}

TEST_CASE(multiplying_two_matrices) {
	matrix4_t a = matrix4(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 8, 7, 6,
			5, 4, 3, 2
			);
	matrix4_t b = matrix4(
			-2, 1, 2, 3,
			3, 2, 1, -1,
			4, 3, 6, 5,
			1, 2, 7, 8
			);
	matrix4_t exp = matrix4(
			20, 22, 50, 48,
			44, 54, 114, 108,
			40, 58, 110, 102,
			16, 26, 46, 42
			);
	ASSERT_TRUE(matrix4_eq(matrix4_mul(a,b), exp));
}

TEST_CASE(matrix4_multiply_tuple) {
	matrix4_t A = matrix4(
			1, 2, 3, 4,
			2, 4, 4, 2,
			8, 6, 4, 1,
			0, 0, 0, 1
			);
	vec4_t b = tuple(1, 2, 3, 1);
	ASSERT_TRUE(vec4_eq(matrix4_mul_vec4(A, b), tuple(18, 24, 33, 1)));
}

TEST_CASE(multiply_by_identity) {
	matrix4_t A = matrix4(
			0, 1, 2, 4,
			1, 2, 4, 8,
			2, 4, 8, 16,
			4, 8, 16, 32
			);
	matrix4_t I = matrix4_identity();
	ASSERT_TRUE(matrix4_eq(matrix4_mul(A, I), A));
}

TEST_CASE(multiply_vec__by_identity) {
	matrix4_t I = matrix4_identity();
	vec4_t a = tuple(1, 2, 3, 4);
	ASSERT_TRUE(vec4_eq(matrix4_mul_vec4(I, a), a));
}

TEST_CASE(transpose_matrix4) {
	matrix4_t A = matrix4(
			0, 9, 3, 0,
			9, 8, 0, 8,
			1, 8, 5, 3,
			0, 0, 5, 8
			);
	matrix4_t exp = matrix4(
			0, 9, 1, 0,
			9, 8, 8, 0,
			3, 0, 5, 5,
			0, 8, 3, 8
			);
	ASSERT_TRUE(matrix4_eq(matrix4_transpose(A), exp));
}

TEST_CASE(transpose_identity_matrix4) {
	matrix4_t I = matrix4_identity();
	ASSERT_TRUE(matrix4_eq(matrix4_transpose(I), I));
}

TEST_CASE(invertible_matrix4) {
	matrix4_t a = matrix4(
			6, 4, 4, 4,
			5, 5, 7, 6,
			4, -9, 3, -7,
			9, 1, 7, -6
			);
	ASSERT_TRUE(req(matrix4_determinant(a), -2120));
	ASSERT_TRUE(matrix4_is_invertible(a));
}

TEST_CASE(noninvertible_matrix4) {
	matrix4_t a = matrix4(
			-4, 2, -2, -3,
			9, 6, 2, 6,
			0, -5, 1, -5,
			0, 0, 0, 0
			);
	ASSERT_TRUE(req(matrix4_determinant(a), 0));
	ASSERT_FALSE(matrix4_is_invertible(a));
}

TEST_CASE(calculate_matrix4_inverse) {
	matrix4_t A = matrix4(
			-5, 2, 6, -8,
			1, -5, 1, 8,
			7, 7, -6, -7,
			1, -3, 7, 4
			);
	matrix4_t B = matrix4_inverse(A);
	ASSERT_TRUE(req(matrix4_determinant(A), 532));
	ASSERT_TRUE(req(B._32, -160.0/532.0));
	ASSERT_TRUE(req(B._23, 105.0/532.0));

	matrix4_t exp = matrix4(
			0.21805, 0.45113, 0.24060, -0.04511,
			-0.80827, -1.45677, -0.44361, 0.52068,
			-0.07895, -0.22368, -0.05263, 0.19737,
			-0.52256, -0.81391, -0.30075, 0.30639
			);

	ASSERT_TRUE(matrix4_eq(B, exp));
}

TEST_CASE(calculate_matrix4_inverse_more) {
	matrix4_t A = matrix4(
			8, -5, 9, 2,
			7, 5, 6, 1,
			-6, 0, 9, 6,
			-3, 0, -9, -4
			);

	ASSERT_TRUE(matrix4_eq(matrix4_inverse(A), matrix4(
					-0.15385, -0.15385, -0.28205, -0.53846,
					-0.07692,  0.12308,  0.02564,  0.03077,
					0.35897,  0.35897,  0.43590,  0.92308,
					-0.69231, -0.69231, -0.76923, -1.92308
					)));

	A = matrix4(
			9,  3,  0,  9,
			-5, -2, -6, -3,
			-4,  9,  6,  4,
			-7,  6,  6,  2
			);

	ASSERT_TRUE(matrix4_eq(matrix4_inverse(A), matrix4(
					-0.04074, -0.07778,  0.14444, -0.22222,
					-0.07778,  0.03333,  0.36667, -0.33333,
					-0.02901, -0.14630, -0.10926,  0.12963,
					0.17778,  0.06667, -0.26667,  0.33333
					)));
}

TEST_CASE(multiply_product_by_inverse) {
	matrix4_t A = matrix4(
			3, -9,  7,  3,
			3, -8,  2, -9,
			-4,  4,  4,  1,
			-6,  5, -1,  1
			);
	matrix4_t B = matrix4(
			8,  2,  2,  2,
			3, -1,  7,  0,
			7,  0,  5,  4,
			6, -2,  0,  5
			);
	matrix4_t C = matrix4_mul(A, B);
	ASSERT_TRUE(matrix4_eq(matrix4_mul(C, matrix4_inverse(B)), A));
}
