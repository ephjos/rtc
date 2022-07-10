
#include "labrat.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "rtc.h"

void plane_intersect(shape_t s, ray_t r, intersection_list_t *ilist) {
  // local ray
  ray_t lr = ray_transform(r, s.inverse_transform);

	if (fabs(lr.direction.y) < EPSILON) {
		return;
	}


	intersection_t i;
	i.t = -lr.origin.y / lr.direction.y;
	i.s = s;

  add_intersection(ilist, i);
}

vec4_t plane_normal_at(shape_t s, vec4_t p) {
  vec4_t object_point = matrix4_mul_vec4(s.inverse_transform, p);
  vec4_t object_normal = vector(0, 1, 0);
  vec4_t world_normal = matrix4_mul_vec4(matrix4_transpose(s.inverse_transform), object_normal);
  world_normal.w = 0;
  return vec4_normalize(world_normal);
}

shape_t plane() {
	shape_t s;
	s.transform = matrix4_identity();
	s.inverse_transform = matrix4_identity();
	s.material = material_default();
	s.instance = calloc(1, sizeof(plane_t));
	s.intersect = plane_intersect;
	s.normal_at = plane_normal_at;
	return s;
}

TEST_CASE(the_normal_of_plane_is_constant) {
	shape_t p = plane();
	vec4_t n1 = plane_normal_at(p, point(0, 0, 0));
	vec4_t n2 = plane_normal_at(p, point(10, 0, -10));
	vec4_t n3 = plane_normal_at(p, point(-5, 0, 150));

	ASSERT_TRUE(vec4_eq(n1, vector(0, 1, 0)));
	ASSERT_TRUE(vec4_eq(n2, vector(0, 1, 0)));
	ASSERT_TRUE(vec4_eq(n3, vector(0, 1, 0)));
	shape_free(p);
}

TEST_CASE(intersect_ray_parallel_plane) {
	shape_t p = plane();
	ray_t r = ray(point(0, 10, 0), vector(0, 0, 1));
	intersection_list_t ilist = intersection_list();
	plane_intersect(p, r, &ilist);

	ASSERT_TRUE(ilist.hit == NULL);
	intersection_list_free(ilist);
	shape_free(p);
}

TEST_CASE(intersect_ray_coplanar_plane) {
	shape_t p = plane();
	ray_t r = ray(point(0, 0, 0), vector(0, 0, 1));
	intersection_list_t ilist = intersection_list();
	plane_intersect(p, r, &ilist);

	ASSERT_TRUE(ilist.hit == NULL);
	intersection_list_free(ilist);
	shape_free(p);
}

TEST_CASE(intersect_ray_plane_above) {
	shape_t p = plane();
	ray_t r = ray(point(0, 1, 0), vector(0, -1, 0));
	intersection_list_t ilist = intersection_list();
	plane_intersect(p, r, &ilist);

	ASSERT_TRUE(ilist.size == 1);
  ASSERT_TRUE(req(ilist.items[0].t, 1));
	ASSERT_TRUE(shape_eq(ilist.items[0].s, p));
	intersection_list_free(ilist);
	shape_free(p);
}

TEST_CASE(intersect_ray_plane_below) {
	shape_t p = plane();
	ray_t r = ray(point(0, -1, 0), vector(0, 1, 0));
	intersection_list_t ilist = intersection_list();
	plane_intersect(p, r, &ilist);

	ASSERT_TRUE(ilist.size == 1);
  ASSERT_TRUE(req(ilist.items[0].t, 1));
	ASSERT_TRUE(shape_eq(ilist.items[0].s, p));
	intersection_list_free(ilist);
	shape_free(p);
}

void plane_demo() {
	shape_t floor = plane();
	shape_set_transform(&floor, scaling(10, 0.01, 10));
	floor.material.color = color(1, 0.9, 0.9);
	floor.material.specular = 0;

	shape_t middle = sphere();
	shape_set_transform(&middle, translation(-0.5, 0, 0.5));
	middle.material.color = color(0.1, 1, 0.5);
	middle.material.diffuse = 0.7;
	middle.material.specular = 0.3;

	shape_t right = sphere();
	matrix4_t ts2[] = {
		scaling(0.5, 0.5, 0.5),
		translation(1.5, 0.5, -0.5),
	};
	shape_set_transform(&right, transform(ts2, 2));
	right.material.color = color(0.5, 1, 0.1);
	right.material.diffuse = 0.7;
	right.material.specular = 0.3;

	shape_t left = sphere();
	matrix4_t ts3[] = {
		scaling(0.33, 0.33, 0.33),
		translation(-1.5, 0.33, -0.75),
	};
	shape_set_transform(&left, transform(ts3, 2));
	left.material.color = color(1, 0.8, 0.1);
	left.material.diffuse = 0.7;
	left.material.specular = 0.3;

	point_light_t light_source = point_light(point(-10, 10, -10), color(1, 1, 1));

	world_t w = world();
	world_add_point_light(&w, light_source);

	world_add_shape(&w, floor);
	world_add_shape(&w, left);
	world_add_shape(&w, middle);
	world_add_shape(&w, right);

	camera_t c = camera(200, 100, M_PI/3);
	c.transform = view_transform(point(0, 1.5, -5), point(0, 1, 0), vector(0, 1, 0));

	canvas_t cvs = camera_render(c, w);

	canvas_save(cvs, "./plane_demo.ppm");

	canvas_free(cvs);
	world_free(w);
}
