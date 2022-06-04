
#include "labrat.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "canvas.h"
#include "error.h"
#include "matrix.h"
#include "transform.h"
#include "util.h"
#include "vec4.h"

#define PI_2 (M_PI / 2)
#define PI_4 (M_PI / 4)

#define R2_2 (0.7071067811865476) // sqrt(2)/2


matrix_t translation(float x, float y, float z) {
	matrix_t m;
	m._00 = 1; m._01 = 0; m._02 = 0; m._03 = x;
	m._10 = 0; m._11 = 1; m._12 = 0; m._13 = y;
	m._20 = 0; m._21 = 0; m._22 = 1; m._23 = z;
	m._30 = 0; m._31 = 0; m._32 = 0; m._33 = 1;
	return m;
}

matrix_t scaling(float x, float y, float z) {
	matrix_t m;
	m._00 = x; m._01 = 0; m._02 = 0; m._03 = 0;
	m._10 = 0; m._11 = y; m._12 = 0; m._13 = 0;
	m._20 = 0; m._21 = 0; m._22 = z; m._23 = 0;
	m._30 = 0; m._31 = 0; m._32 = 0; m._33 = 1;
	return m;
}

matrix_t rotate_x(float r) {
	matrix_t m;
	m._00 = 1; m._01 = 0; m._02 = 0; m._03 = 0;
	m._10 = 0; m._11 = cos(r); m._12 = -sin(r); m._13 = 0;
	m._20 = 0; m._21 = sin(r); m._22 = cos(r); m._23 = 0;
	m._30 = 0; m._31 = 0; m._32 = 0; m._33 = 1;
	return m;
}

matrix_t rotate_y(float r) {
	matrix_t m;
	m._00 = cos(r); m._01 = 0; m._02 = sin(r); m._03 = 0;
	m._10 = 0; m._11 = 1; m._12 = 0; m._13 = 0;
	m._20 = -sin(r); m._21 = 0; m._22 = cos(r); m._23 = 0;
	m._30 = 0; m._31 = 0; m._32 = 0; m._33 = 1;
	return m;
}

matrix_t rotate_z(float r) {
	matrix_t m;
	m._00 = cos(r); m._01 = -sin(r); m._02 = 0; m._03 = 0;
	m._10 = sin(r); m._11 = cos(r); m._12 = 0; m._13 = 0;
	m._20 = 0; m._21 = 0; m._22 = 1; m._23 = 0;
	m._30 = 0; m._31 = 0; m._32 = 0; m._33 = 1;
	return m;
}

matrix_t shearing(float a, float b, float c, float d, float e, float f) {
	matrix_t m;
	m._00 = 1; m._01 = a; m._02 = b; m._03 = 0;
	m._10 = c; m._11 = 1; m._12 = d; m._13 = 0;
	m._20 = e; m._21 = f; m._22 = 1; m._23 = 0;
	m._30 = 0; m._31 = 0; m._32 = 0; m._33 = 1;
	return m;
}

matrix_t transform(matrix_t *ms, int n) {
	matrix_t res = matrix_identity();
	for (int i = 0; i < n; i++) {
		res = matrix_mul(ms[i], res);
	}
	return res;
}

TEST_CASE(multiplying_by_translation) {
	vec4_t p = point(-3, 4, 5);
	matrix_t transform = translation(5, -3, 2);

	vec4_t res = matrix_mul_vec4(transform, p);
	ASSERT_TRUE(vec4_eq(res, point(2, 1, 7)));
}

TEST_CASE(multiplying_by_translation_inverse) {
	vec4_t p = point(-3, 4, 5);
	matrix_t transform = matrix_inverse(translation(5, -3, 2));

	vec4_t res = matrix_mul_vec4(transform, p);
	ASSERT_TRUE(vec4_eq(res, point(-8, 7, 3)));
}

TEST_CASE(cannot_translate_vectors) {
	vec4_t v = vector(-3, 4, 5);
	matrix_t transform = translation(5, -3, 2);

	vec4_t res = matrix_mul_vec4(transform, v);
	ASSERT_TRUE(vec4_eq(res, v));
}

TEST_CASE(scaling_point) {
	vec4_t p = point(-4, 6, 8);
	matrix_t transform = scaling(2, 3, 4);

	vec4_t res = matrix_mul_vec4(transform, p);
	ASSERT_TRUE(vec4_eq(res, point(-8, 18, 32)));
}

TEST_CASE(scaling_vector) {
	vec4_t v = vector(-4, 6, 8);
	matrix_t transform = scaling(2, 3, 4);

	vec4_t res = matrix_mul_vec4(transform, v);
	ASSERT_TRUE(vec4_eq(res, vector(-8, 18, 32)));
}

TEST_CASE(multiplying_by_scaling_inverse) {
	vec4_t v = vector(-4, 6, 8);
	matrix_t transform = matrix_inverse(scaling(2, 3, 4));

	vec4_t res = matrix_mul_vec4(transform, v);
	ASSERT_TRUE(vec4_eq(res, vector(-2, 2, 2)));
}

TEST_CASE(reflection_is_scaling_negative) {
	vec4_t p = point(2, 3, 4);
	matrix_t transform = scaling(-1, 1, 1);

	vec4_t res = matrix_mul_vec4(transform, p);
	ASSERT_TRUE(vec4_eq(res, point(-2, 3, 4)));
}

TEST_CASE(rotate_point_around_x_axis) {
	vec4_t p = point(0, 1, 0);
	matrix_t half_quarter = rotate_x(PI_4);
	matrix_t full_quarter = rotate_x(PI_2);

	ASSERT_TRUE(vec4_eq(matrix_mul_vec4(half_quarter, p), point(0, R2_2, R2_2)));
	ASSERT_TRUE(vec4_eq(matrix_mul_vec4(full_quarter, p), point(0, 0, 1)));
}

TEST_CASE(rotate_point_around_x_axis_inverse) {
	vec4_t p = point(0, 1, 0);
	matrix_t inv_half_quarter = matrix_inverse(rotate_x(PI_4));

	ASSERT_TRUE(vec4_eq(matrix_mul_vec4(inv_half_quarter, p), point(0, R2_2, -R2_2)));
}

TEST_CASE(rotate_point_around_y_axis) {
	vec4_t p = point(0, 0, 1);
	matrix_t half_quarter = rotate_y(PI_4);
	matrix_t full_quarter = rotate_y(PI_2);

	ASSERT_TRUE(vec4_eq(matrix_mul_vec4(half_quarter, p), point(R2_2, 0, R2_2)));
	ASSERT_TRUE(vec4_eq(matrix_mul_vec4(full_quarter, p), point(1, 0, 0)));
}

TEST_CASE(rotate_point_around_z_axis) {
	vec4_t p = point(0, 1, 0);
	matrix_t half_quarter = rotate_z(PI_4);
	matrix_t full_quarter = rotate_z(PI_2);

	ASSERT_TRUE(vec4_eq(matrix_mul_vec4(half_quarter, p), point(-R2_2, R2_2, 0)));
	ASSERT_TRUE(vec4_eq(matrix_mul_vec4(full_quarter, p), point(-1, 0, 0)));
}

TEST_CASE(shearing_a) {
	vec4_t p = point(2, 3, 4);
	matrix_t transform = shearing(1, 0, 0, 0, 0, 0);
	ASSERT_TRUE(vec4_eq(matrix_mul_vec4(transform, p), point(5, 3, 4)));
}

TEST_CASE(shearing_b) {
	vec4_t p = point(2, 3, 4);
	matrix_t transform = shearing(0, 1, 0, 0, 0, 0);
	ASSERT_TRUE(vec4_eq(matrix_mul_vec4(transform, p), point(6, 3, 4)));
}

TEST_CASE(shearing_c) {
	vec4_t p = point(2, 3, 4);
	matrix_t transform = shearing(0, 0, 1, 0, 0, 0);
	ASSERT_TRUE(vec4_eq(matrix_mul_vec4(transform, p), point(2, 5, 4)));
}

TEST_CASE(shearing_d) {
	vec4_t p = point(2, 3, 4);
	matrix_t transform = shearing(0, 0, 0, 1, 0, 0);
	ASSERT_TRUE(vec4_eq(matrix_mul_vec4(transform, p), point(2, 7, 4)));
}

TEST_CASE(shearing_e) {
	vec4_t p = point(2, 3, 4);
	matrix_t transform = shearing(0, 0, 0, 0, 1, 0);
	ASSERT_TRUE(vec4_eq(matrix_mul_vec4(transform, p), point(2, 3, 6)));
}

TEST_CASE(shearing_f) {
	vec4_t p = point(2, 3, 4);
	matrix_t transform = shearing(0, 0, 0, 0, 0, 1);
	ASSERT_TRUE(vec4_eq(matrix_mul_vec4(transform, p), point(2, 3, 7)));
}

TEST_CASE(transform_sequence) {
	vec4_t p = point(1, 0, 1);
	matrix_t A = rotate_x(PI_2);
	matrix_t B = scaling(5, 5, 5);
	matrix_t C = translation(10, 5, 7);

	p = matrix_mul_vec4(A, p);
	ASSERT_TRUE(vec4_eq(p, point(1, -1, 0)));

	p = matrix_mul_vec4(B, p);
	ASSERT_TRUE(vec4_eq(p, point(5, -5, 0)));

	p = matrix_mul_vec4(C, p);
	ASSERT_TRUE(vec4_eq(p, point(15, 0, 7)));
}

TEST_CASE(chained_sequence) {
	vec4_t p = point(1, 0, 1);
	matrix_t A = rotate_x(PI_2);
	matrix_t B = scaling(5, 5, 5);
	matrix_t C = translation(10, 5, 7);
	matrix_t T = matrix_mul(C, matrix_mul(B, A));

	p = matrix_mul_vec4(T, p);
	ASSERT_TRUE(vec4_eq(p, point(15, 0, 7)));
}

TEST_CASE(transform_func) {
	vec4_t p = point(1, 0, 1);
	matrix_t ts[] = {
		rotate_x(PI_2),
		scaling(5, 5, 5),
		translation(10, 5, 7),
	};
	matrix_t T = transform(ts, 3);

	p = matrix_mul_vec4(T, p);
	ASSERT_TRUE(vec4_eq(p, point(15, 0, 7)));
}


void transform_demo() {
	void canvas_write_tick(canvas_t* c, int x, int y, color_t p) {
		const int w = 5;
		for (int i = -w; i <= w; i++) {
			for (int j = -w; j <= w; j++) {
				int ii = x-i;
				int jj = y-j;
				if (ii >= 0 && ii < c->width && jj >= 0 && jj < c->height) {
					canvas_write(c, ii, jj, p);
				}
			}
		}
	}

	color_t red = color(1, 0, 0);

	const int SIZE = 256;
	canvas_t c = canvas(SIZE, SIZE);

	vec4_t p = point(0, 0, SIZE/4);
	const float STEP = (2*M_PI)/12;

	for (int i = 0; i < 12; i++) {
		matrix_t rotate = rotate_y(i*STEP);
		vec4_t rp = matrix_mul_vec4(rotate, p);

		canvas_write_tick(&c, rp.x+(SIZE/2), rp.z+(SIZE/2), red);
	}

	canvas_save(c, "./transform_demo.ppm");
	canvas_free(c);
}
