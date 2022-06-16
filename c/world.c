
#include "labrat.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "color.h"
#include "ray.h"
#include "sphere.h"
#include "transform.h"
#include "vec4.h"
#include "world.h"
#include "util.h"

world_t world() {
	world_t w;

	w.n_point_lights = 0;
	w.point_lights = NULL;

	w.n_spheres = 0;
	w.spheres = NULL;

	return w;
}

world_t default_world() {
	world_t w;

	w.n_point_lights = 1;
	w.point_lights = (point_light_t*)calloc(1, sizeof(point_light_t));
	w.point_lights[0] = point_light(point(-10, 10, -10), color(1, 1, 1));

	w.n_spheres = 2;
	w.spheres = (sphere_t*)calloc(2, sizeof(sphere_t));
	w.spheres[0] = sphere(0);
	w.spheres[0].material.color = color(0.8, 1, 0.6);
	w.spheres[0].material.diffuse = 0.7;
	w.spheres[0].material.specular = 0.2;
	w.spheres[1] = sphere(1);
	sphere_set_transform(&w.spheres[1], scaling(0.5, 0.5, 0.5));

	return w;
}

intersection_list_t world_intersect_ray(world_t w, ray_t r) {
	intersection_list_t ilist = intersection_list();

	for (int i = 0; i < w.n_spheres; i++) {
		sphere_intersect(w.spheres[i], r, &ilist);
	}

	return ilist;
}

TEST_CASE(creating_a_world) {
	world_t w = world();
	ASSERT_TRUE(w.n_point_lights == 0);
	ASSERT_TRUE(w.n_spheres == 0);

	ASSERT_TRUE(w.point_lights == NULL);
	ASSERT_TRUE(w.spheres == NULL);
}

TEST_CASE(the_default_world) {
	point_light_t light = point_light(point(-10, 10, -10), color(1, 1, 1));

	sphere_t s1 = sphere(0);
	s1.material.color = color(0.8, 1, 0.6);
	s1.material.diffuse = 0.7;
	s1.material.specular = 0.2;

	sphere_t s2 = sphere(1);
	sphere_set_transform(&s2, scaling(0.5, 0.5, 0.5));

	world_t w = default_world();
	ASSERT_TRUE(w.n_point_lights == 1);
	ASSERT_TRUE(w.n_spheres == 2);

	ASSERT_TRUE(point_light_eq(w.point_lights[0], light));
	ASSERT_TRUE(sphere_eq(w.spheres[0], s1));
	ASSERT_TRUE(sphere_eq(w.spheres[1], s2));
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
}
