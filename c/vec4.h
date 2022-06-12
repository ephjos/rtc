#pragma once

#include <stdbool.h>

typedef struct vec4_t {
	float x;
	float y;
	float z;
	float w;
} vec4_t;

static const vec4_t VEC4_ZERO = {0, 0, 0, 0};

void vec4_print(vec4_t v);

vec4_t tuple(float x, float y, float z, float w);
vec4_t point(float x, float y, float z);
vec4_t vector(float x, float y, float z);
bool vec4_is_point(vec4_t v);
bool vec4_is_vector(vec4_t v);
bool vec4_eq(vec4_t a, vec4_t b);

vec4_t vec4_add(vec4_t a, vec4_t b);
vec4_t vec4_sub(vec4_t a, vec4_t b);
vec4_t vec4_neg(vec4_t a);
vec4_t vec4_muls(vec4_t a, float s);
vec4_t vec4_divs(vec4_t a, float s);

float vec4_magnitude(vec4_t v);
vec4_t vec4_normalize(vec4_t v);
float vec4_dot(vec4_t a, vec4_t b);
vec4_t vec4_cross(vec4_t a, vec4_t b);
vec4_t vec4_reflect(vec4_t in, vec4_t normal);

void vec4_demo();

