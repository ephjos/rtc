
#include "labrat.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "camera.h"
#include "ray.h"
#include "shape.h"
#include "transform.h"
#include "vec4.h"
#include "world.h"
#include "util.h"

camera_t camera(int hsize, int vsize, float fov) {
	camera_t c;
	c.hsize = hsize;
	c.vsize = vsize;
	c.fov = fov;
	c.transform = matrix4_identity();

	float half_view = tan(c.fov/2);
	float aspect = (float)c.hsize / (float)c.vsize;

	if (aspect >= 1) {
		c.half_width = half_view;
		c.half_height = half_view / aspect;
	} else {
		c.half_width = half_view * aspect;
		c.half_height = half_view;
	}

	c.pixel_size = (c.half_width*2) / c.hsize;

	return c;
}

ray_t camera_ray_for_pixel(camera_t c, int x, int y) {
	ray_t r;

	float xoffset = (x + 0.5) * c.pixel_size;
	float yoffset = (y + 0.5) * c.pixel_size;

	float world_x = c.half_width - xoffset;
	float world_y = c.half_height - yoffset;

	vec4_t pixel = matrix4_mul_vec4(matrix4_inverse(c.transform), point(world_x, world_y, -1));
	vec4_t origin = matrix4_mul_vec4(matrix4_inverse(c.transform), point(0, 0, 0));
	vec4_t direction = vec4_normalize(vec4_sub(pixel, origin));

	return ray(origin, direction);
}

canvas_t camera_render(camera_t c, world_t w) {
	canvas_t cvs = canvas(c.hsize, c.vsize);

	for (int y = 0; y < c.vsize; y++) {
		for (int x = 0; x < c.hsize; x++) {
			ray_t r = camera_ray_for_pixel(c, x, y);
			color_t col = world_color_at(w, r);
			canvas_write(&cvs, x, y, col);
		}
	}

	return cvs;
}

TEST_CASE(constructing_a_camera) {
	int hsize = 160;
	int vsize = 120;
	float fov = M_PI/2;
	camera_t c = camera(hsize, vsize, fov);

	ASSERT_TRUE(req(c.hsize, 160));
	ASSERT_TRUE(req(c.vsize, 120));
	ASSERT_TRUE(req(c.fov, M_PI/2));
	ASSERT_TRUE(matrix4_eq(c.transform, matrix4_identity()));
}

TEST_CASE(pixel_size_horizontal) {
	camera_t c = camera(200, 125, M_PI/2);
	ASSERT_TRUE(req(c.pixel_size, 0.01));
}

TEST_CASE(pixel_size_vertical) {
	camera_t c = camera(125, 200, M_PI/2);
	ASSERT_TRUE(req(c.pixel_size, 0.01));
}

TEST_CASE(ray_through_center) {
	camera_t c = camera(201, 101, M_PI/2);
	ray_t r = camera_ray_for_pixel(c, 100, 50);

	ASSERT_TRUE(vec4_eq(r.origin, point(0, 0, 0)));
	ASSERT_TRUE(vec4_eq(r.direction, vector(0, 0, -1)));
}

TEST_CASE(ray_through_corner) {
	camera_t c = camera(201, 101, M_PI/2);
	ray_t r = camera_ray_for_pixel(c, 0, 0);

	ASSERT_TRUE(vec4_eq(r.origin, point(0, 0, 0)));
	ASSERT_TRUE(vec4_eq(r.direction, vector(0.66519, 0.33259, -0.66851)));
}

TEST_CASE(ray_when_transformed) {
	camera_t c = camera(201, 101, M_PI/2);
	matrix4_t ts[] = {
		translation(0, -2, 5),
		rotate_y(M_PI/4),
	};
	c.transform = transform(ts, 2);
	ray_t r = camera_ray_for_pixel(c, 100, 50);

	ASSERT_TRUE(vec4_eq(r.origin, point(0, 2, -5)));
	ASSERT_TRUE(vec4_eq(r.direction, vector(sqrtf(2)/2, 0, -sqrtf(2)/2)));
}

TEST_CASE(rendering_a_world) {
	world_t w = default_world();
	camera_t c = camera(11, 11, M_PI/2);
	vec4_t from = point(0, 0, -5);
	vec4_t to = point(0, 0, 0);
	vec4_t up = vector(0, 1, 0);
	c.transform = view_transform(from, to, up);
	canvas_t cvs = camera_render(c, w);

	ASSERT_TRUE(color_eq(canvas_get(cvs, 5, 5), color(0.38066, 0.47583, 0.2855)));

	canvas_free(cvs);
	world_free(w);
}

void camera_demo() {
	shape_t floor = sphere();
	shape_set_transform(&floor, scaling(10, 0.01, 10));
	floor.material.color = color(1, 0.9, 0.9);
	floor.material.specular = 0;

	shape_t left_wall = sphere();
	matrix4_t ts[] = {
		scaling(10, 0.01, 10),
		rotate_x(M_PI/2),
		rotate_y(-M_PI/4),
		translation(0, 0, 5),
	};
	shape_set_transform(&left_wall, transform(ts, 4));
	left_wall.material = floor.material;

	shape_t right_wall = sphere();
	matrix4_t ts1[] = {
		scaling(10, 0.01, 10),
		rotate_x(M_PI/2),
		rotate_y(M_PI/4),
		translation(0, 0, 5),
	};
	shape_set_transform(&right_wall, transform(ts1, 4));
	right_wall.material = floor.material;

	shape_t middle = sphere();
	shape_set_transform(&middle, translation(-0.5, 1, 0.5));
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
	w.n_point_lights = 1;
	w.point_lights = calloc(w.n_point_lights, sizeof(point_light_t));
	w.point_lights[0] = light_source;

	w.n_shapes = 6;
	w.shapes = calloc(w.n_shapes, sizeof(shape_t));
	w.shapes[0] = floor;
	w.shapes[1] = left_wall;
	w.shapes[2] = right_wall;
	w.shapes[3] = left;
	w.shapes[4] = middle;
	w.shapes[5] = right;

	camera_t c = camera(1000, 500, M_PI/3);
	c.transform = view_transform(point(0, 1.5, -5), point(0, 1, 0), vector(0, 1, 0));

	canvas_t cvs = camera_render(c, w);

	canvas_save(cvs, "./camera_demo.ppm");

	canvas_free(cvs);
	world_free(w);
}
