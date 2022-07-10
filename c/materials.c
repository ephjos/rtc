
#include "labrat.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "rtc.h"

#define R2_2 (0.7071067811865476) // sqrt(2)/2

material_t material_default() {
	material_t m;
	m.color = color(1, 1, 1);
	m.ambient = 0.1;
	m.diffuse = 0.9;
	m.specular = 0.9;
	m.shininess = 200;
	return m;
}

bool material_eq(material_t a, material_t b) {
	return color_eq(a.color, b.color) &&
		req(a.ambient, b.ambient) &&
		req(a.diffuse, b.diffuse) &&
		req(a.specular, b.specular) &&
		req(a.shininess, b.shininess);
}

color_t lighting(
		material_t m,
		point_light_t light,
		vec4_t point,
		vec4_t eyev,
		vec4_t normalv,
		bool in_shadow
		) {
	color_t effective_color = color_mul(m.color, light.intensity);
	vec4_t lightv = vec4_normalize(vec4_sub(light.position, point));
	color_t ambient = color_muls(effective_color, m.ambient);

	double light_dot_normal = vec4_dot(lightv, normalv);

	color_t diffuse = color(0, 0, 0);
	color_t specular = color(0, 0, 0);

	if (!in_shadow) {
		if (light_dot_normal >= 0) {
			diffuse = color_muls(effective_color, m.diffuse*light_dot_normal);
			vec4_t reflectv = vec4_reflect(vec4_neg(lightv), normalv);
			double reflect_dot_eye = vec4_dot(reflectv, eyev);
			if (reflect_dot_eye > 0) {
				double factor = pow(reflect_dot_eye, m.shininess);
				specular = color_muls(light.intensity, m.specular*factor);
			}
		}
	}

	return color_add(ambient, color_add(diffuse, specular));
}

TEST_CASE(the_default_material) {
	material_t m = material_default();

	ASSERT_TRUE(color_eq(m.color, color(1, 1, 1)));
	ASSERT_TRUE(req(m.ambient, 0.1));
	ASSERT_TRUE(req(m.diffuse, 0.9));
	ASSERT_TRUE(req(m.specular, 0.9));
	ASSERT_TRUE(req(m.shininess, 200));
}

TEST_CASE(lighting_eye_between_light_and_surface) {
	material_t m = material_default();
	vec4_t position = point(0, 0, 0);

	vec4_t eyev = vector(0, 0, -1);
	vec4_t normalv = vector(0, 0, -1);
	point_light_t light = point_light(point(0, 0, -10), color(1, 1, 1));

	color_t result = lighting(m, light, position, eyev, normalv, false);
	ASSERT_TRUE(color_eq(result, color(1.9, 1.9, 1.9)));
}

TEST_CASE(lighting_eye_between_light_and_surface_with_45_degree_offset) {
	material_t m = material_default();
	vec4_t position = point(0, 0, 0);

	vec4_t eyev = vector(0, R2_2, -R2_2);
	vec4_t normalv = vector(0, 0, -1);
	point_light_t light = point_light(point(0, 0, -10), color(1, 1, 1));

	color_t result = lighting(m, light, position, eyev, normalv, false);
	ASSERT_TRUE(color_eq(result, color(1.0, 1.0, 1.0)));
}

TEST_CASE(lighting_eye_opposite_surface_light_45_degree_offset) {
	material_t m = material_default();
	vec4_t position = point(0, 0, 0);

	vec4_t eyev = vector(0, 0, -1);
	vec4_t normalv = vector(0, 0, -1);
	point_light_t light = point_light(point(0, 10, -10), color(1, 1, 1));

	color_t result = lighting(m, light, position, eyev, normalv, false);
	ASSERT_TRUE(color_eq(result, color(0.7364, 0.7364, 0.7364)));
}

TEST_CASE(lighting_eye_in_reflection_path) {
	material_t m = material_default();
	vec4_t position = point(0, 0, 0);

	vec4_t eyev = vector(0, -R2_2, -R2_2);
	vec4_t normalv = vector(0, 0, -1);
	point_light_t light = point_light(point(0, 10, -10), color(1, 1, 1));

	color_t result = lighting(m, light, position, eyev, normalv, false);
	ASSERT_TRUE(color_eq(result, color(1.6364, 1.6364, 1.6364)));
}

TEST_CASE(lighting_with_light_behind_surface) {
	material_t m = material_default();
	vec4_t position = point(0, 0, 0);

	vec4_t eyev = vector(0, 0, -1);
	vec4_t normalv = vector(0, 0, -1);
	point_light_t light = point_light(point(0, 0, 10), color(1, 1, 1));

	color_t result = lighting(m, light, position, eyev, normalv, false);
	ASSERT_TRUE(color_eq(result, color(0.1, 0.1, 0.1)));
}

TEST_CASE(lighting_surface_in_shadow) {
	material_t m = material_default();
	vec4_t position = point(0, 0, 0);

	vec4_t eyev = vector(0, 0, -1);
	vec4_t normalv = vector(0, 0, -1);
	point_light_t light = point_light(point(0, 0, -10), color(1, 1, 1));

	color_t result = lighting(m, light, position, eyev, normalv, true);
	ASSERT_TRUE(color_eq(result, color(0.1, 0.1, 0.1)));
}

void material_demo() {
	vec4_t from = point(0, 0, 0);
	shape_t s = sphere();
	s.material.color = color(1, 0.2, 1);

	matrix4_t st[] = {
		translation(0, 0, 7),
		scaling(1, 2, 1),
		rotate_z(0.5),
	};

	shape_set_transform(&s, transform(st, 1));

	vec4_t light_position = point(-10, 10, -10);
	color_t light_color = color(1, 1, 1);
	point_light_t pl = point_light(light_position, light_color);

	color_t red = color(1, 0, 0);
	const double WALL_Z = 14;
	const double WALL_SIZE = 10;
	const double SIZE = 128;
	canvas_t c = canvas(SIZE, SIZE);
	const double PIXEL_SIZE = WALL_SIZE / SIZE;

	for (int i = 0; i < SIZE; i++) {
		double y = (WALL_SIZE/2) - PIXEL_SIZE * i;
		for (int j = 0; j < SIZE; j++) {
			double x = (-WALL_SIZE/2) + PIXEL_SIZE * j;
			ray_t r = ray(from, vec4_normalize(vec4_sub(point(x, y, WALL_Z), from)));
			intersection_list_t ilist = intersection_list();
			sphere_intersect(s, r, &ilist);
			if (ilist.hit != NULL) {
				vec4_t p = ray_position(r, ilist.hit->t);
				vec4_t n = sphere_normal_at(s, p);
				vec4_t eye = vec4_neg(r.direction);
				canvas_write(&c, j, i, lighting(s.material, pl, p, eye, n, false));
			}
			intersection_list_free(ilist);
		}
	}

  shape_free(s);

	canvas_save(c, "./material_demo.ppm");
	canvas_free(c);
}
