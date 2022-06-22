
#include "labrat.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "color.h"
#include "error.h"
#include "ray.h"
#include "shape.h"
#include "transform.h"
#include "vec4.h"
#include "world.h"
#include "util.h"

world_t world() {
	world_t w;

	w.n_point_lights = 0;
	w.cap_point_lights = 16;
	w.point_lights = (point_light_t*)calloc(w.cap_point_lights, sizeof(point_light_t));

	w.n_shapes = 0;
	w.cap_shapes = 16;
	w.shapes = (shape_t*)calloc(w.cap_shapes, sizeof(shape_t));

	return w;
}

void world_add_point_light(world_t *w, point_light_t pl) {
	if (w->n_point_lights >= w->cap_point_lights - 1) {
		int oc = w->cap_point_lights;
		w->cap_point_lights *= 2;
		w->point_lights = (point_light_t*)realloc(
				w->point_lights, sizeof(point_light_t)*w->cap_point_lights);
		if (w->point_lights == NULL) {
			fprintf(stderr, "Unable to grow world point_lights from %d to %d", oc, w->cap_point_lights);
			exit(ERROR_GROW_WORLD_POINT_LIGHTS_LIST);
			return;
		}
	}

	w->point_lights[w->n_point_lights++] = pl;
}

void world_add_shape(world_t *w, shape_t s) {
	if (w->n_shapes >= w->cap_shapes - 1) {
		int oc = w->cap_shapes;
		w->cap_shapes *= 2;
		w->shapes = (shape_t*)realloc(
				w->shapes, sizeof(shape_t)*w->cap_shapes);
		if (w->shapes == NULL) {
			fprintf(stderr, "Unable to grow world shapes from %d to %d", oc, w->cap_shapes);
			exit(ERROR_GROW_WORLD_POINT_LIGHTS_LIST);
			return;
		}
	}

	w->shapes[w->n_shapes++] = s;
}

void world_free(world_t w) {
	free(w.point_lights);
	for (int i = 0; i < w.n_shapes; i++) {
		shape_free(w.shapes[i]);
	}
	free(w.shapes);
}

world_t default_world() {
	world_t w = world();
	world_add_point_light(&w, point_light(point(-10, 10, -10), color(1, 1, 1)));

	shape_t s1 = sphere();
	s1.material.color = color(0.8, 1, 0.6);
	s1.material.diffuse = 0.7;
	s1.material.specular = 0.2;
	world_add_shape(&w, s1);

	shape_t s2 = sphere();
	shape_set_transform(&s2, scaling(0.5, 0.5, 0.5));
	world_add_shape(&w, s2);

	return w;
}

intersection_list_t world_intersect_ray(world_t w, ray_t r) {
	intersection_list_t ilist = intersection_list();

	for (int i = 0; i < w.n_shapes; i++) {
		w.shapes[i].intersect(w.shapes[i], r, &ilist);
	}

	return ilist;
}

color_t world_shade_hit(world_t w, computations_t c) {
	color_t res = color(0, 0, 0);

	for (int i = 0; i < w.n_point_lights; i++) {
		bool is_shadowed = world_point_is_shadowed(w, c.over_point, i);
		color_t hit = lighting(
					c.s.material,
					w.point_lights[i],
					c.over_point,
					c.eyev,
					c.normalv,
					is_shadowed
					);
		res = color_add(res, hit);
	}

	return res;
}

color_t world_color_at(world_t w, ray_t r) {
	intersection_list_t ilist = world_intersect_ray(w, r);
	if (ilist.hit == NULL) {
		intersection_list_free(ilist);
		return color(0, 0, 0);
	}

	computations_t comps = prepare_computations(*ilist.hit, r);
	intersection_list_free(ilist);
	return world_shade_hit(w, comps);
}

bool world_point_is_shadowed(world_t w, vec4_t p, int point_lights_i) {
	vec4_t v = vec4_sub(w.point_lights[point_lights_i].position, p);
	double distance = vec4_magnitude(v);
	vec4_t direction = vec4_normalize(v);

	ray_t r = ray(p, direction);
	intersection_list_t ilist = world_intersect_ray(w, r);

	bool res = ilist.hit != NULL && ilist.hit->t < distance;
	intersection_list_free(ilist);
	return res;
}

TEST_CASE(creating_a_world) {
	world_t w = world();
	ASSERT_TRUE(w.n_point_lights == 0);
	ASSERT_TRUE(w.n_shapes == 0);
	world_free(w);
}

TEST_CASE(the_default_world) {
	point_light_t light = point_light(point(-10, 10, -10), color(1, 1, 1));

	shape_t s1 = sphere();
	s1.material.color = color(0.8, 1, 0.6);
	s1.material.diffuse = 0.7;
	s1.material.specular = 0.2;

	shape_t s2 = sphere();
	shape_set_transform(&s2, scaling(0.5, 0.5, 0.5));

	world_t w = default_world();
	ASSERT_TRUE(w.n_point_lights == 1);
	ASSERT_TRUE(w.n_shapes == 2);

	ASSERT_TRUE(point_light_eq(w.point_lights[0], light));
	ASSERT_TRUE(shape_eq(w.shapes[0], s1));
	ASSERT_TRUE(shape_eq(w.shapes[1], s2));

	shape_free(s1);
	shape_free(s2);
	world_free(w);
}

TEST_CASE(intersect_world_with_a_ray) {
	world_t w = default_world();
	ray_t r = ray(point(0, 0, -5), vector(0, 0, 1));
	intersection_list_t ilist = world_intersect_ray(w, r);

	ASSERT_TRUE(ilist.size == 4);
	ASSERT_TRUE(req(ilist.items[0].t, 4));
	ASSERT_TRUE(req(ilist.items[1].t, 4.5));
	ASSERT_TRUE(req(ilist.items[2].t, 5.5));
	ASSERT_TRUE(req(ilist.items[3].t, 6));
	intersection_list_free(ilist);
	world_free(w);
}

TEST_CASE(shading_an_intersection) {
	world_t w = default_world();
	ray_t r = ray(point(0, 0, -5), vector(0, 0, 1));
	shape_t s = w.shapes[0];
	intersection_t i = {
		.t = 4,
		.s = s,
	};
	computations_t comps = prepare_computations(i, r);
	color_t c = world_shade_hit(w, comps);
	ASSERT_TRUE(color_eq(c, color(0.38066, 0.47583, 0.2855)));
	world_free(w);
}

TEST_CASE(shading_an_intersection_from_the_inside) {
	world_t w = default_world();
	w.point_lights[0] = point_light(point(0, 0.25, 0), color(1, 1, 1));
	ray_t r = ray(point(0, 0, 0), vector(0, 0, 1));
	shape_t s = w.shapes[1];
	intersection_t i = {
		.t = 0.5,
		.s = s,
	};
	computations_t comps = prepare_computations(i, r);
	color_t c = world_shade_hit(w, comps);
	ASSERT_TRUE(color_eq(c, color(0.90498, 0.90498, 0.90498)));
	world_free(w);
}

TEST_CASE(the_color_when_a_ray_misses) {
	world_t w = default_world();
	ray_t r = ray(point(0, 0, -5), vector(0, 1, 0));
	color_t c = world_color_at(w, r);
	ASSERT_TRUE(color_eq(c, color(0, 0, 0)));
	world_free(w);
}

TEST_CASE(the_color_when_a_ray_hits) {
	world_t w = default_world();
	ray_t r = ray(point(0, 0, -5), vector(0, 0, 1));
	color_t c = world_color_at(w, r);
	ASSERT_TRUE(color_eq(c, color(0.38066, 0.47583, 0.2855)));
	world_free(w);
}

TEST_CASE(the_color_with_an_intersection_behind_the_ray) {
	world_t w = default_world();
	w.shapes[0].material.ambient = 1;
	w.shapes[1].material.ambient = 1;
	ray_t r = ray(point(0, 0, 0.75), vector(0, 0, -1));
	color_t c = world_color_at(w, r);
	ASSERT_TRUE(color_eq(c, w.shapes[1].material.color));
	world_free(w);
}

TEST_CASE(there_is_no_shadow_when_nothing_colinear) {
	world_t w = default_world();
	vec4_t p = point(0, 10, 0);

	ASSERT_FALSE(world_point_is_shadowed(w, p, 0));
	world_free(w);
}

TEST_CASE(there_is_shadow_when_object_between) {
	world_t w = default_world();
	vec4_t p = point(10, -10, 10);

	ASSERT_TRUE(world_point_is_shadowed(w, p, 0));
	world_free(w);
}

TEST_CASE(there_is_no_shadow_when_object_is_behind_light) {
	world_t w = default_world();
	vec4_t p = point(-20, 20, -20);

	ASSERT_FALSE(world_point_is_shadowed(w, p, 0));
	world_free(w);
}

TEST_CASE(there_is_no_shadow_when_object_is_behind_point) {
	world_t w = default_world();
	vec4_t p = point(-2, 2, -2);

	ASSERT_FALSE(world_point_is_shadowed(w, p, 0));
	world_free(w);
}

TEST_CASE(shade_hit_is_given_an_intersection_in_shadow) {
	world_t w = world();
	world_add_point_light(&w, point_light(point(0, 0, -10), color(1, 1, 1)));

	shape_t s1 = sphere();
	world_add_shape(&w, s1);

	shape_t s2 = sphere();
	shape_set_transform(&s2, translation(0, 0, 10));
	world_add_shape(&w, s2);

	ray_t r = ray(point(0, 0, 5), vector(0, 0, 1));
	intersection_t i = {
		.t = 4,
		.s = s2,
	};
	computations_t comps = prepare_computations(i, r);
	color_t c = world_shade_hit(w, comps);

	ASSERT_TRUE(color_eq(c, color(0.1, 0.1, 0.1)));
	world_free(w);
}
