#pragma once

#include "lights.h"
#include "ray.h"
#include "shape.h"

typedef struct world_t {
	point_light_t *point_lights;
	int n_point_lights;

	shape_t *shapes;
	int n_shapes;
} world_t;

world_t world();
void world_free(world_t w);
world_t default_world();

intersection_list_t world_intersect_ray(world_t w, ray_t r);
