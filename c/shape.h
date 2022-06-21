#pragma once

#include "materials.h"
#include "matrix4.h"
#include "ray.h"

// Predefine types
typedef struct shape_t shape_t;
typedef struct sphere_t sphere_t;
typedef struct intersection_t intersection_t;
typedef struct intersection_list_t intersection_list_t;
typedef struct computations_t computations_t;

// Shape
typedef struct shape_t {
	matrix4_t transform;
	matrix4_t inverse_transform;
	material_t material;
	void *instance;
	void (*intersect)(shape_t s, ray_t r, intersection_list_t *ilist);
	vec4_t (*normal_at)(shape_t s, vec4_t p);
	bool (*eq)(shape_t a, shape_t b);
} shape_t;

bool shape_eq(shape_t a, shape_t b);
void shape_set_transform(shape_t *s, matrix4_t t);
void shape_free(shape_t s);

// Sphere

typedef struct sphere_t {
} sphere_t;

void sphere_intersect(shape_t s, ray_t r, intersection_list_t *ilist);
vec4_t sphere_normal_at(shape_t s, vec4_t p);
shape_t sphere();

// Intersections
typedef struct intersection_t {
	float t;
	shape_t s;
} intersection_t;

typedef struct intersection_list_t {
	int size;
	int cap;
	intersection_t *items;
	intersection_t *hit;
} intersection_list_t;

#define INTERSECTION_LIST_INITIAL_CAP 16

// Caller responsible for freeing
intersection_list_t intersection_list();

void intersection_list_free(intersection_list_t ilist);
void add_intersection(intersection_list_t *ilist, intersection_t item);
intersection_t* hit(intersection_list_t ilist);

// Computations

typedef struct computations_t {
	float t;
	shape_t s;
	vec4_t point;
	vec4_t eyev;
	vec4_t normalv;
	bool inside;
} computations_t;

computations_t prepare_computations(intersection_t i, ray_t r);
