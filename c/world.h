#pragma once

#include "intersection.h"
#include "lights.h"
#include "ray.h"
#include "sphere.h"

typedef struct world_t {
	point_light_t *point_lights;
	int n_point_lights;

	sphere_t *spheres;
	int n_spheres;
} world_t;

world_t world();
world_t default_world();

intersection_list_t world_intersect_ray(world_t w, ray_t r);
