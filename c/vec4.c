
#include "labrat.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vec4.h"
#include "util.h"

void vec4_print(vec4_t v) {
	printf("[%0.2f, %0.2f, %0.2f, %0.2f]\n", v.x, v.y, v.z, v.w);
}

vec4_t tuple(double x, double y, double z, double w) {
	vec4_t v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
	return v;
}

vec4_t point(double x, double y, double z) {
	return tuple(x, y, z, 1.0);
}
vec4_t vector(double x, double y, double z) {
	return tuple(x, y, z, 0.0);
}

bool vec4_is_point(vec4_t v) {
	return req(v.w, 1);
}

bool vec4_is_vector(vec4_t v) {
	return req(v.w, 0);
}

bool vec4_eq(vec4_t a, vec4_t b) {
	return req(a.x, b.x) && req(a.y, b.y) && req(a.z, b.z) && req(a.w, b.w);
}

vec4_t vec4_add(vec4_t a, vec4_t b) {
	return tuple(
			a.x + b.x,
			a.y + b.y,
			a.z + b.z,
			a.w + b.w
			);
}

vec4_t vec4_sub(vec4_t a, vec4_t b) {
	return tuple(
			a.x - b.x,
			a.y - b.y,
			a.z - b.z,
			a.w - b.w
			);
}

vec4_t vec4_neg(vec4_t a) {
	return vec4_sub(VEC4_ZERO, a);
}

vec4_t vec4_muls(vec4_t a, double s) {
	return tuple(
			a.x * s,
			a.y * s,
			a.z * s,
			a.w * s
			);
}

vec4_t vec4_divs(vec4_t a, double s) {
	double d = 1.0 / s;
	return tuple(
			a.x * d,
			a.y * d,
			a.z * d,
			a.w * d
			);
}

double vec4_magnitude(vec4_t v) {
	return sqrt(
			(v.x*v.x) +
			(v.y*v.y) +
			(v.z*v.z) +
			(v.w*v.w)
			);
}

vec4_t vec4_normalize(vec4_t v) {
	return vec4_divs(v, vec4_magnitude(v));
}

double vec4_dot(vec4_t a, vec4_t b) {
	return (a.x*b.x)+(a.y*b.y)+(a.z*b.z)+(a.w*b.w);
}

vec4_t vec4_cross(vec4_t a, vec4_t b) {
	return vector(
			a.y*b.z - a.z*b.y,
			a.z*b.x - a.x*b.z,
			a.x*b.y - a.y*b.x
			);
}

vec4_t vec4_reflect(vec4_t in, vec4_t normal) {
	return vec4_sub(in, vec4_muls(normal, 2*vec4_dot(in, normal)));
}

TEST_CASE(a_tuple_with_w_1_is_a_point) {
	vec4_t a = tuple(4.3, -4.2, 3.1, 1.0);
	ASSERT_TRUE(req(a.x, 4.3));
	ASSERT_TRUE(req(a.y, -4.2));
	ASSERT_TRUE(req(a.z, 3.1));
	ASSERT_TRUE(req(a.w, 1.0));
	ASSERT_TRUE(vec4_is_point(a));
	ASSERT_TRUE(!vec4_is_vector(a));
}

TEST_CASE(a_tuple_with_w_0_is_a_vector) {
	vec4_t a = tuple(4.3, -4.2, 3.1, 0.0);
	ASSERT_TRUE(req(a.x, 4.3));
	ASSERT_TRUE(req(a.y, -4.2));
	ASSERT_TRUE(req(a.z, 3.1));
	ASSERT_TRUE(req(a.w, 0.0));
	ASSERT_TRUE(!vec4_is_point(a));
	ASSERT_TRUE(vec4_is_vector(a));
}

TEST_CASE(point_create_tuples_with_w_1) {
	vec4_t p = point(4, -4, 3);
	ASSERT_TRUE(vec4_eq(p, tuple(4, -4, 3, 1)));
}

TEST_CASE(vector_create_tuples_with_w_0) {
	vec4_t v = vector(4, -4, 3);
	ASSERT_TRUE(vec4_eq(v, tuple(4, -4, 3, 0)));
}

TEST_CASE(adding_two_tuples) {
	vec4_t a = tuple(3, -2, 5, 1);
	vec4_t b = tuple(-2, 3, 1, 0);
	vec4_t res = vec4_add(a, b);
	ASSERT_TRUE(vec4_eq(res, tuple(1,1,6,1)));
}

TEST_CASE(subtracting_two_tuples) {
	vec4_t p1 = point(3, 2, 1);
	vec4_t p2 = point(5, 6, 7);
	vec4_t res = vec4_sub(p1, p2);
	ASSERT_TRUE(vec4_eq(res, vector(-2, -4, -6)));
}

TEST_CASE(subtracting_a_vector_from_a_point) {
	vec4_t p = point(3, 2, 1);
	vec4_t v = vector(5, 6, 7);
	vec4_t res = vec4_sub(p, v);
	ASSERT_TRUE(vec4_eq(res, point(-2, -4, -6)));
}

TEST_CASE(subtracting_two_vectors) {
	vec4_t v1 = vector(3, 2, 1);
	vec4_t v2 = vector(5, 6, 7);
	vec4_t res = vec4_sub(v1, v2);
	ASSERT_TRUE(vec4_eq(res, vector(-2, -4, -6)));
}

TEST_CASE(subtracting_vector_from_zero_vector) {
	vec4_t zero = vector(0, 0, 0);
	vec4_t v = vector(1, -2, 3);
	vec4_t res = vec4_sub(zero, v);
	ASSERT_TRUE(vec4_eq(res, vector(-1, 2, -3)));
}

TEST_CASE(negating_a_tuple) {
	vec4_t a = tuple(1, -2, 3, -4);
	vec4_t res = vec4_neg(a);
	ASSERT_TRUE(vec4_eq(res, tuple(-1, 2, -3, 4)));
}

TEST_CASE(multiplying_tuple_by_a_scalar) {
	vec4_t a = tuple(1, -2, 3, -4);
	vec4_t res = vec4_muls(a, 3.5);
	ASSERT_TRUE(vec4_eq(res, tuple(3.5, -7, 10.5, -14)));
}

TEST_CASE(multiplying_tuple_by_a_fraction) {
	vec4_t a = tuple(1, -2, 3, -4);
	vec4_t res = vec4_muls(a, 0.5);
	ASSERT_TRUE(vec4_eq(res, tuple(0.5, -1, 1.5, -2)));
}

TEST_CASE(dividing_tuple_by_scalar) {
	vec4_t a = tuple(1, -2, 3, -4);
	vec4_t res = vec4_divs(a, 2);
	ASSERT_TRUE(vec4_eq(res, tuple(0.5, -1, 1.5, -2)));
}

TEST_CASE(compute_magnitude_of_vectors) {
	vec4_t v = vector(1, 0, 0);
	ASSERT_TRUE(req(vec4_magnitude(v), 1));

	v = vector(0, 1, 0);
	ASSERT_TRUE(req(vec4_magnitude(v), 1));

	v = vector(0, 0, 1);
	ASSERT_TRUE(req(vec4_magnitude(v), 1));

	v = vector(1, 2, 3);
	ASSERT_TRUE(req(vec4_magnitude(v), sqrt(14)));

	v = vector(-1, -2, -3);
	ASSERT_TRUE(req(vec4_magnitude(v), sqrt(14)));
}

TEST_CASE(normalize_vectors) {
	vec4_t v = vector(4, 0, 0);
	ASSERT_TRUE(vec4_eq(vec4_normalize(v), vector(1, 0, 0)));

	double r14 = sqrt(14);
	v = vector(1, 2, 3);
	ASSERT_TRUE(vec4_eq(vec4_normalize(v), vector(1/r14, 2/r14, 3/r14)));
}

TEST_CASE(magnitude_of_normalize_vectors) {
	vec4_t v = vector(1, 2, 3);
	vec4_t norm = vec4_normalize(v);
	ASSERT_TRUE(req(vec4_magnitude(norm), 1));
}

TEST_CASE(the_dot_product_of_two_tuples) {
	vec4_t a = vector(1, 2, 3);
	vec4_t b = vector(2, 3, 4);
	ASSERT_TRUE(req(vec4_dot(a, b), 20));
}

TEST_CASE(the_cross_product_of_two_vectors) {
	vec4_t a = vector(1, 2, 3);
	vec4_t b = vector(2, 3, 4);
	ASSERT_TRUE(vec4_eq(vec4_cross(a, b), vector(-1, 2, -1)));
	ASSERT_TRUE(vec4_eq(vec4_cross(b, a), vector(1, -2, 1)));
}

typedef struct env_t {
	vec4_t gravity;
	vec4_t wind;
} env_t;
typedef struct proj_t {
	vec4_t position;
	vec4_t velocity;
} proj_t;

static proj_t tick(env_t e, proj_t p) {
	proj_t proj;
	proj.position = vec4_add(p.position, p.velocity);
	proj.velocity = vec4_add(vec4_add(p.velocity, e.gravity), e.wind);
	return proj;
}

void vec4_demo() {
	proj_t p;
	p.position = point(0, 1, 0);
	p.velocity = vec4_normalize(vector(1, 1, 0));

	env_t e;
	e.gravity = vector(0, -0.1, 0);
	e.wind = vector(-0.01, 0, 0);

	while (p.position.y > 0) {
		vec4_print(p.position);
		p = tick(e, p);
	}

	vec4_print(p.position);
}

TEST_CASE(reflecting_incoming_45_degrees) {
	vec4_t v = vector(1, -1, 0);
	vec4_t n = vector(0, 1, 0);
	vec4_t r = vec4_reflect(v, n);

	ASSERT_TRUE(vec4_eq(r, vector(1,1,0)));
}

TEST_CASE(reflecting_off_slanted_surface) {
	vec4_t v = vector(0, -1, 0);
	vec4_t n = vector(sqrtf(2)/2, sqrtf(2)/2, 0);
	vec4_t r = vec4_reflect(v, n);

	ASSERT_TRUE(vec4_eq(r, vector(1,0,0)));
}
