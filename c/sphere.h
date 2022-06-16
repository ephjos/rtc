#pragma once

#include "intersection.h"
#include "materials.h"
#include "matrix4.h"
#include "ray.h"

typedef struct sphere_t {
	int id;
	matrix4_t transform;
	matrix4_t inverse_transform;
	material_t material;
} sphere_t;

sphere_t sphere();
bool sphere_eq(sphere_t a, sphere_t b);

void sphere_intersect(sphere_t s, ray_t r, intersection_list_t *ilist);
void sphere_set_transform(sphere_t *s, matrix4_t t);

vec4_t sphere_normal_at(sphere_t s, vec4_t p);

