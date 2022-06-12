#pragma once

#include "intersection.h"
#include "matrix4.h"
#include "ray.h"

typedef struct sphere_t {
	int id;
	matrix4_t transform;
	matrix4_t inverse_transform;
} sphere_t;

sphere_t sphere();

intersection_list_t sphere_intersect(sphere_t s, ray_t r);
void sphere_set_transform(sphere_t *s, matrix4_t t);

