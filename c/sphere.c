
#include "labrat.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "rtc.h"

void sphere_intersect(shape_t s, ray_t r, intersection_list_t *ilist) {
  // local ray
  ray_t lr = ray_transform(r, s.inverse_transform);

  vec4_t sphere_to_ray = vec4_sub(lr.origin, point(0, 0, 0));

  double a = vec4_dot(lr.direction, lr.direction);
  double b = 2 * vec4_dot(lr.direction, sphere_to_ray);
  double c = vec4_dot(sphere_to_ray, sphere_to_ray) - 1;

  double discriminant = (b*b) - 4 * a * c;

  if (discriminant < 0) {
    return;
  }

  double sqrtd = sqrtf(discriminant);

  intersection_t i1;
  i1.t = (-b - sqrtd) / (2*a);
	i1.s = s;

  intersection_t i2;
  i2.t = (-b + sqrtd) / (2*a);
	i2.s = s;

  add_intersection(ilist, i1);
  add_intersection(ilist, i2);
  return;
}

vec4_t sphere_normal_at(shape_t s, vec4_t p) {
  vec4_t object_point = matrix4_mul_vec4(s.inverse_transform, p);
  vec4_t object_normal = vec4_sub(object_point, point(0, 0, 0));
  vec4_t world_normal = matrix4_mul_vec4(matrix4_transpose(s.inverse_transform), object_normal);
  world_normal.w = 0;
  return vec4_normalize(world_normal);
}

shape_t sphere() {
	shape_t s;
	s.transform = matrix4_identity();
	s.inverse_transform = matrix4_identity();
	s.material = material_default();
	s.instance = calloc(1, sizeof(sphere_t));
	s.intersect = sphere_intersect;
	s.normal_at = sphere_normal_at;
	return s;
}

TEST_CASE(ray_intersects_sphere_at_two_point) {
  ray_t r = ray(point(0, 0, -5), vector(0, 0, 1));
  shape_t s = sphere();
  intersection_list_t ilist = intersection_list();
	sphere_intersect(s, r, &ilist);

  ASSERT_EQ(ilist.size, 2, "%d");
  ASSERT_TRUE(req(ilist.items[0].t, 4.0));
  ASSERT_TRUE(req(ilist.items[1].t, 6.0));

  intersection_list_free(ilist);
	shape_free(s);
}

TEST_CASE(ray_intersects_sphere_at_tangent) {
  ray_t r = ray(point(0, 1, -5), vector(0, 0, 1));
  shape_t s = sphere();
  intersection_list_t ilist = intersection_list();
	sphere_intersect(s, r, &ilist);

  ASSERT_EQ(ilist.size, 2, "%d");
  ASSERT_TRUE(req(ilist.items[0].t, 5.0));
  ASSERT_TRUE(req(ilist.items[1].t, 5.0));

  intersection_list_free(ilist);
	shape_free(s);
}

TEST_CASE(ray_misses_sphere) {
  ray_t r = ray(point(0, 2, -5), vector(0, 0, 1));
  shape_t s = sphere();
  intersection_list_t ilist = intersection_list();
	sphere_intersect(s, r, &ilist);

  ASSERT_EQ(ilist.size, 0, "%d");

  intersection_list_free(ilist);
	shape_free(s);
}

TEST_CASE(ray_inside_sphere) {
  ray_t r = ray(point(0, 0, 0), vector(0, 0, 1));
  shape_t s = sphere();
  intersection_list_t ilist = intersection_list();
	sphere_intersect(s, r, &ilist);

  ASSERT_EQ(ilist.size, 2, "%d");
  ASSERT_TRUE(req(ilist.items[0].t, -1.0));
  ASSERT_TRUE(req(ilist.items[1].t, 1.0));

  intersection_list_free(ilist);
	shape_free(s);
}

TEST_CASE(ray_behind_sphere) {
  ray_t r = ray(point(0, 0, 5), vector(0, 0, 1));
  shape_t s = sphere();
  intersection_list_t ilist = intersection_list();
	sphere_intersect(s, r, &ilist);

  ASSERT_EQ(ilist.size, 2, "%d");
  ASSERT_TRUE(req(ilist.items[0].t, -6.0));
  ASSERT_TRUE(req(ilist.items[1].t, -4.0));

  intersection_list_free(ilist);
	shape_free(s);
}


TEST_CASE(intersect_sets_object) {
  ray_t r = ray(point(0, 0, -5), vector(0, 0, 1));
  shape_t s = sphere();
  intersection_list_t ilist = intersection_list();
	sphere_intersect(s, r, &ilist);

  ASSERT_EQ(ilist.size, 2, "%d");
	ASSERT_TRUE(shape_eq(ilist.items[0].s, s));
	ASSERT_TRUE(shape_eq(ilist.items[1].s, s));

  intersection_list_free(ilist);

	shape_free(s);
}

TEST_CASE(sphere_default_transformation) {
  shape_t s = sphere();

  ASSERT_TRUE(matrix4_eq(s.transform, matrix4_identity()));

	shape_free(s);
}

TEST_CASE(sphere_set_the_transform) {
  shape_t s = sphere();
  matrix4_t t = translation(2, 3, 4);
  shape_set_transform(&s, t);

  ASSERT_TRUE(matrix4_eq(s.transform, t));
  ASSERT_TRUE(matrix4_eq(s.inverse_transform, matrix4_inverse(t)));

	shape_free(s);
}

TEST_CASE(intersecting_a_scaled_sphere) {
  ray_t r = ray(point(0, 0, -5), vector(0, 0, 1));
  shape_t s = sphere();
  shape_set_transform(&s, scaling(2, 2, 2));

  intersection_list_t ilist = intersection_list();
	sphere_intersect(s, r, &ilist);
  ASSERT_EQ(ilist.size, 2, "%d");
  ASSERT_TRUE(req(ilist.items[0].t, 3));
  ASSERT_TRUE(req(ilist.items[1].t, 7));

  intersection_list_free(ilist);
	shape_free(s);
}

TEST_CASE(intersecting_a_translated_sphere) {
  ray_t r = ray(point(0, 0, -5), vector(0, 0, 1));
  shape_t s = sphere();
  shape_set_transform(&s, translation(5, 0, 0));

  intersection_list_t ilist = intersection_list();
	sphere_intersect(s, r, &ilist);
  ASSERT_EQ(ilist.size, 0, "%d");

  intersection_list_free(ilist);
	shape_free(s);
}

TEST_CASE(sphere_normal_on_x_axis) {
  shape_t s = sphere();
  vec4_t n = sphere_normal_at(s, point(1, 0, 0));

  ASSERT_TRUE(vec4_eq(n, vector(1, 0, 0)));

	shape_free(s);
}

TEST_CASE(sphere_normal_on_y_axis) {
  shape_t s = sphere();
  vec4_t n = sphere_normal_at(s, point(0, 1, 0));

  ASSERT_TRUE(vec4_eq(n, vector(0, 1, 0)));

	shape_free(s);
}

TEST_CASE(sphere_normal_on_z_axis) {
  shape_t s = sphere();
  vec4_t n = sphere_normal_at(s, point(0, 0, 1));

  ASSERT_TRUE(vec4_eq(n, vector(0, 0, 1)));

	shape_free(s);
}

TEST_CASE(sphere_normal_on_nonaxial_point) {
  double r3_3 = sqrtf(3) / 3;
  shape_t s = sphere();
  vec4_t n = sphere_normal_at(s, point(r3_3, r3_3, r3_3));

  ASSERT_TRUE(vec4_eq(n, vector(r3_3, r3_3, r3_3)));

	shape_free(s);
}

TEST_CASE(sphere_normal_is_normalized) {
  double r3_3 = sqrtf(3) / 3;
  shape_t s = sphere();
  vec4_t n = sphere_normal_at(s, point(r3_3, r3_3, r3_3));

  ASSERT_TRUE(vec4_eq(n, vec4_normalize(n)));

	shape_free(s);
}

TEST_CASE(sphere_normal_when_translated) {
  shape_t s = sphere();
  shape_set_transform(&s, translation(0, 1, 0));
  vec4_t n = sphere_normal_at(s, point(0, 1.70711, -0.70711));

  ASSERT_TRUE(vec4_eq(n, vector(0, 0.70711, -0.70711)));

	shape_free(s);
}

TEST_CASE(sphere_normal_when_transformed) {
  shape_t s = sphere();
  matrix4_t ts[] = {
    rotate_z(M_PI / 5),
    scaling(1, 0.5, 1),
  };
  shape_set_transform(&s, transform(ts, 2));
  vec4_t n = sphere_normal_at(s, point(0, sqrtf(2)/2, -sqrtf(2)/2));

  ASSERT_TRUE(vec4_eq(n, vector(0, 0.97014, -0.24254)));

	shape_free(s);
}

TEST_CASE(sphere_has_default_material) {
  shape_t s = sphere();
  material_t m = s.material;

  ASSERT_TRUE(material_eq(m, material_default()));

	shape_free(s);
}

TEST_CASE(sphere_may_be_assigned_material) {
  shape_t s = sphere();
  material_t m = material_default();
  m.ambient = 1;
  s.material = m;

  ASSERT_TRUE(material_eq(s.material, m));

	shape_free(s);
}
