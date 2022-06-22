#pragma once

#include <stdbool.h>

#include "color.h"
#include "lights.h"
#include "ray.h"
#include "shape.h"

typedef struct world_t {
	int n_point_lights;
	int cap_point_lights;
	point_light_t *point_lights;

	int n_shapes;
	int cap_shapes;
	shape_t *shapes;
} world_t;

world_t world();
void world_add_point_light(world_t *w, point_light_t pl);
void world_add_shape(world_t *w, shape_t s);
void world_free(world_t w);

world_t default_world();

intersection_list_t world_intersect_ray(world_t w, ray_t r);
color_t world_shade_hit(world_t w, computations_t c);
color_t world_color_at(world_t w, ray_t r);
bool world_point_is_shadowed(world_t w, vec4_t p, int point_lights_i);
