
#include "labrat.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "canvas.h"
#include "error.h"
#include "matrix4.h"
#include "transform.h"
#include "util.h"
#include "vec4.h"

#define PI_2 (M_PI / 2)
#define PI_4 (M_PI / 4)

#define R2_2 (0.7071067811865476) // sqrt(2)/2


matrix4_t translation(float x, float y, float z) {
	matrix4_t m;
	m._00 = 1; m._01 = 0; m._02 = 0; m._03 = x;
	m._10 = 0; m._11 = 1; m._12 = 0; m._13 = y;
	m._20 = 0; m._21 = 0; m._22 = 1; m._23 = z;
	m._30 = 0; m._31 = 0; m._32 = 0; m._33 = 1;
	return m;
}

matrix4_t scaling(float x, float y, float z) {
	matrix4_t m;
	m._00 = x; m._01 = 0; m._02 = 0; m._03 = 0;
	m._10 = 0; m._11 = y; m._12 = 0; m._13 = 0;
	m._20 = 0; m._21 = 0; m._22 = z; m._23 = 0;
	m._30 = 0; m._31 = 0; m._32 = 0; m._33 = 1;
	return m;
}

matrix4_t rotate_x(float r) {
	matrix4_t m;
	m._00 = 1; m._01 = 0; m._02 = 0; m._03 = 0;
	m._10 = 0; m._11 = cos(r); m._12 = -sin(r); m._13 = 0;
	m._20 = 0; m._21 = sin(r); m._22 = cos(r); m._23 = 0;
	m._30 = 0; m._31 = 0; m._32 = 0; m._33 = 1;
	return m;
}

matrix4_t rotate_y(float r) {
	matrix4_t m;
	m._00 = cos(r); m._01 = 0; m._02 = sin(r); m._03 = 0;
	m._10 = 0; m._11 = 1; m._12 = 0; m._13 = 0;
	m._20 = -sin(r); m._21 = 0; m._22 = cos(r); m._23 = 0;
	m._30 = 0; m._31 = 0; m._32 = 0; m._33 = 1;
	return m;
}

matrix4_t rotate_z(float r) {
	matrix4_t m;
	m._00 = cos(r); m._01 = -sin(r); m._02 = 0; m._03 = 0;
	m._10 = sin(r); m._11 = cos(r); m._12 = 0; m._13 = 0;
	m._20 = 0; m._21 = 0; m._22 = 1; m._23 = 0;
	m._30 = 0; m._31 = 0; m._32 = 0; m._33 = 1;
	return m;
}

matrix4_t shearing(float a, float b, float c, float d, float e, float f) {
	matrix4_t m;
	m._00 = 1; m._01 = a; m._02 = b; m._03 = 0;
	m._10 = c; m._11 = 1; m._12 = d; m._13 = 0;
	m._20 = e; m._21 = f; m._22 = 1; m._23 = 0;
	m._30 = 0; m._31 = 0; m._32 = 0; m._33 = 1;
	return m;
}

matrix4_t transform(matrix4_t *ms, int n) {
	matrix4_t res = matrix4_identity();
	for (int i = 0; i < n; i++) {
		res = matrix4_mul(ms[i], res);
	}
	return res;
}

matrix4_t view_transform(vec4_t from, vec4_t to, vec4_t up) {
	vec4_t forward = vec4_normalize(vec4_sub(to, from));
	vec4_t upn = vec4_normalize(up);
	vec4_t left = vec4_cross(forward, upn);
	vec4_t true_up = vec4_cross(left, forward);
	matrix4_t orientation = matrix4(
			left.x, left.y, left.z, 0,
			true_up.x, true_up.y, true_up.z, 0,
			-forward.x, -forward.y, -forward.z, 0,
			0, 0, 0, 1
			);
	return matrix4_mul(orientation, translation(-from.x, -from.y, -from.z));
}

TEST_CASE(multiplying_by_translation) {
	vec4_t p = point(-3, 4, 5);
	matrix4_t transform = translation(5, -3, 2);

	vec4_t res = matrix4_mul_vec4(transform, p);
	ASSERT_TRUE(vec4_eq(res, point(2, 1, 7)));
}

TEST_CASE(multiplying_by_translation_inverse) {
	vec4_t p = point(-3, 4, 5);
	matrix4_t transform = matrix4_inverse(translation(5, -3, 2));

	vec4_t res = matrix4_mul_vec4(transform, p);
	ASSERT_TRUE(vec4_eq(res, point(-8, 7, 3)));
}

TEST_CASE(cannot_translate_vectors) {
	vec4_t v = vector(-3, 4, 5);
	matrix4_t transform = translation(5, -3, 2);

	vec4_t res = matrix4_mul_vec4(transform, v);
	ASSERT_TRUE(vec4_eq(res, v));
}

TEST_CASE(scaling_point) {
	vec4_t p = point(-4, 6, 8);
	matrix4_t transform = scaling(2, 3, 4);

	vec4_t res = matrix4_mul_vec4(transform, p);
	ASSERT_TRUE(vec4_eq(res, point(-8, 18, 32)));
}

TEST_CASE(scaling_vector) {
	vec4_t v = vector(-4, 6, 8);
	matrix4_t transform = scaling(2, 3, 4);

	vec4_t res = matrix4_mul_vec4(transform, v);
	ASSERT_TRUE(vec4_eq(res, vector(-8, 18, 32)));
}

TEST_CASE(multiplying_by_scaling_inverse) {
	vec4_t v = vector(-4, 6, 8);
	matrix4_t transform = matrix4_inverse(scaling(2, 3, 4));

	vec4_t res = matrix4_mul_vec4(transform, v);
	ASSERT_TRUE(vec4_eq(res, vector(-2, 2, 2)));
}

TEST_CASE(reflection_is_scaling_negative) {
	vec4_t p = point(2, 3, 4);
	matrix4_t transform = scaling(-1, 1, 1);

	vec4_t res = matrix4_mul_vec4(transform, p);
	ASSERT_TRUE(vec4_eq(res, point(-2, 3, 4)));
}

TEST_CASE(rotate_point_around_x_axis) {
	vec4_t p = point(0, 1, 0);
	matrix4_t half_quarter = rotate_x(PI_4);
	matrix4_t full_quarter = rotate_x(PI_2);

	ASSERT_TRUE(vec4_eq(matrix4_mul_vec4(half_quarter, p), point(0, R2_2, R2_2)));
	ASSERT_TRUE(vec4_eq(matrix4_mul_vec4(full_quarter, p), point(0, 0, 1)));
}

TEST_CASE(rotate_point_around_x_axis_inverse) {
	vec4_t p = point(0, 1, 0);
	matrix4_t inv_half_quarter = matrix4_inverse(rotate_x(PI_4));

	ASSERT_TRUE(vec4_eq(matrix4_mul_vec4(inv_half_quarter, p), point(0, R2_2, -R2_2)));
}

TEST_CASE(rotate_point_around_y_axis) {
	vec4_t p = point(0, 0, 1);
	matrix4_t half_quarter = rotate_y(PI_4);
	matrix4_t full_quarter = rotate_y(PI_2);

	ASSERT_TRUE(vec4_eq(matrix4_mul_vec4(half_quarter, p), point(R2_2, 0, R2_2)));
	ASSERT_TRUE(vec4_eq(matrix4_mul_vec4(full_quarter, p), point(1, 0, 0)));
}

TEST_CASE(rotate_point_around_z_axis) {
	vec4_t p = point(0, 1, 0);
	matrix4_t half_quarter = rotate_z(PI_4);
	matrix4_t full_quarter = rotate_z(PI_2);

	ASSERT_TRUE(vec4_eq(matrix4_mul_vec4(half_quarter, p), point(-R2_2, R2_2, 0)));
	ASSERT_TRUE(vec4_eq(matrix4_mul_vec4(full_quarter, p), point(-1, 0, 0)));
}

TEST_CASE(shearing_a) {
	vec4_t p = point(2, 3, 4);
	matrix4_t transform = shearing(1, 0, 0, 0, 0, 0);
	ASSERT_TRUE(vec4_eq(matrix4_mul_vec4(transform, p), point(5, 3, 4)));
}

TEST_CASE(shearing_b) {
	vec4_t p = point(2, 3, 4);
	matrix4_t transform = shearing(0, 1, 0, 0, 0, 0);
	ASSERT_TRUE(vec4_eq(matrix4_mul_vec4(transform, p), point(6, 3, 4)));
}

TEST_CASE(shearing_c) {
	vec4_t p = point(2, 3, 4);
	matrix4_t transform = shearing(0, 0, 1, 0, 0, 0);
	ASSERT_TRUE(vec4_eq(matrix4_mul_vec4(transform, p), point(2, 5, 4)));
}

TEST_CASE(shearing_d) {
	vec4_t p = point(2, 3, 4);
	matrix4_t transform = shearing(0, 0, 0, 1, 0, 0);
	ASSERT_TRUE(vec4_eq(matrix4_mul_vec4(transform, p), point(2, 7, 4)));
}

TEST_CASE(shearing_e) {
	vec4_t p = point(2, 3, 4);
	matrix4_t transform = shearing(0, 0, 0, 0, 1, 0);
	ASSERT_TRUE(vec4_eq(matrix4_mul_vec4(transform, p), point(2, 3, 6)));
}

TEST_CASE(shearing_f) {
	vec4_t p = point(2, 3, 4);
	matrix4_t transform = shearing(0, 0, 0, 0, 0, 1);
	ASSERT_TRUE(vec4_eq(matrix4_mul_vec4(transform, p), point(2, 3, 7)));
}

TEST_CASE(transform_sequence) {
	vec4_t p = point(1, 0, 1);
	matrix4_t A = rotate_x(PI_2);
	matrix4_t B = scaling(5, 5, 5);
	matrix4_t C = translation(10, 5, 7);

	p = matrix4_mul_vec4(A, p);
	ASSERT_TRUE(vec4_eq(p, point(1, -1, 0)));

	p = matrix4_mul_vec4(B, p);
	ASSERT_TRUE(vec4_eq(p, point(5, -5, 0)));

	p = matrix4_mul_vec4(C, p);
	ASSERT_TRUE(vec4_eq(p, point(15, 0, 7)));
}

TEST_CASE(chained_sequence) {
	vec4_t p = point(1, 0, 1);
	matrix4_t A = rotate_x(PI_2);
	matrix4_t B = scaling(5, 5, 5);
	matrix4_t C = translation(10, 5, 7);
	matrix4_t T = matrix4_mul(C, matrix4_mul(B, A));

	p = matrix4_mul_vec4(T, p);
	ASSERT_TRUE(vec4_eq(p, point(15, 0, 7)));
}

TEST_CASE(transform_func) {
	vec4_t p = point(1, 0, 1);
	matrix4_t ts[] = {
		rotate_x(PI_2),
		scaling(5, 5, 5),
		translation(10, 5, 7),
	};
	matrix4_t T = transform(ts, 3);

	p = matrix4_mul_vec4(T, p);
	ASSERT_TRUE(vec4_eq(p, point(15, 0, 7)));
}

TEST_CASE(view_transform_default) {
	vec4_t from = point(0, 0, 0);
	vec4_t to = point(0, 0, -1);
	vec4_t up = vector(0, 1, 0);

	matrix4_t t = view_transform(from, to, up);
	ASSERT_TRUE(matrix4_eq(t, matrix4_identity()));
}

TEST_CASE(view_transform_positive_z) {
	vec4_t from = point(0, 0, 0);
	vec4_t to = point(0, 0, 1);
	vec4_t up = vector(0, 1, 0);

	matrix4_t t = view_transform(from, to, up);
	ASSERT_TRUE(matrix4_eq(t, scaling(-1, 1, -1)));
}

TEST_CASE(view_transform_moves_world) {
	vec4_t from = point(0, 0, 8);
	vec4_t to = point(0, 0, 0);
	vec4_t up = vector(0, 1, 0);

	matrix4_t t = view_transform(from, to, up);
	ASSERT_TRUE(matrix4_eq(t, translation(0, 0, -8)));
}

TEST_CASE(arbitrary_view_transform) {
	vec4_t from = point(1, 3, 2);
	vec4_t to = point(4, -2, 8);
	vec4_t up = vector(1, 1, 0);

	matrix4_t t = view_transform(from, to, up);
	ASSERT_TRUE(matrix4_eq(t, matrix4(
					-0.50709, 0.50709, 0.67612, -2.36643,
					0.76772, 0.60609, 0.12122, -2.82843,
					-0.35857, 0.59761, -0.71714, 0,
					0, 0, 0, 1
					)));
}

static void canvas_write_tick(canvas_t* c, int x, int y, color_t p) {
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

void transform_demo() {
	color_t red = color(1, 0, 0);

	const float SIZE = 256;
	canvas_t c = canvas(SIZE, SIZE);

	vec4_t p = point(0, 0, SIZE/4);
	const float STEP = (2*M_PI)/12;

	for (int i = 0; i < 12; i++) {
		matrix4_t rotate = rotate_y(i*STEP);
		vec4_t rp = matrix4_mul_vec4(rotate, p);

		canvas_write_tick(&c, rp.x+(SIZE/2), rp.z+(SIZE/2), red);
	}

	canvas_save(c, "./transform_demo.ppm");
	canvas_free(c);
}
