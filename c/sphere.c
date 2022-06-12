
#include "labrat.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "matrix4.h"
#include "ray.h"
#include "shape.h"
#include "sphere.h"
#include "transform.h"
#include "util.h"
#include "vec4.h"

sphere_t sphere(int id) {
  sphere_t s;
  s.id = id;
  s.transform = matrix4_identity();
  s.inverse_transform = matrix4_identity();
  return s;
}

intersection_list_t sphere_intersect(sphere_t s, ray_t r) {
  // local ray
  ray_t lr = ray_transform(r, s.inverse_transform);

  intersection_list_t ilist = intersection_list();

  vec4_t sphere_to_ray = vec4_sub(lr.origin, point(0, 0, 0));

  float a = vec4_dot(lr.direction, lr.direction);
  float b = 2 * vec4_dot(lr.direction, sphere_to_ray);
  float c = vec4_dot(sphere_to_ray, sphere_to_ray) - 1;

  float discriminant = (b*b) - 4 * a * c;

  if (discriminant < 0) {
    return ilist;
  }

  float sqrtd = sqrtf(discriminant);

  intersection_t i1;
  i1.t = (-b - sqrtd) / (2*a);
  i1.type = TYPE_SPHERE;
  i1.id = s.id;

  intersection_t i2;
  i2.t = (-b + sqrtd) / (2*a);
  i2.type = TYPE_SPHERE;
  i2.id = s.id;

  add_intersection(&ilist, i1);
  add_intersection(&ilist, i2);

  return ilist;
}

void sphere_set_transform(sphere_t *s, matrix4_t t) {
  s->transform = t;
  s->inverse_transform = matrix4_inverse(t);
}

TEST_CASE(ray_intersects_sphere_at_two_point) {
  ray_t r = ray(point(0, 0, -5), vector(0, 0, 1));
  sphere_t s = sphere(0);
  intersection_list_t ilist = sphere_intersect(s, r);

  ASSERT_EQ(ilist.size, 2, "%d");
  ASSERT_TRUE(req(ilist.items[0].t, 4.0));
  ASSERT_TRUE(req(ilist.items[1].t, 6.0));
  free_intersection_list(ilist);
}

TEST_CASE(ray_intersects_sphere_at_tangent) {
  ray_t r = ray(point(0, 1, -5), vector(0, 0, 1));
  sphere_t s = sphere(0);
  intersection_list_t ilist = sphere_intersect(s, r);

  ASSERT_EQ(ilist.size, 2, "%d");
  ASSERT_TRUE(req(ilist.items[0].t, 5.0));
  ASSERT_TRUE(req(ilist.items[1].t, 5.0));
  free_intersection_list(ilist);
}

TEST_CASE(ray_misses_sphere) {
  ray_t r = ray(point(0, 2, -5), vector(0, 0, 1));
  sphere_t s = sphere(0);
  intersection_list_t ilist = sphere_intersect(s, r);

  ASSERT_EQ(ilist.size, 0, "%d");
  free_intersection_list(ilist);
}

TEST_CASE(ray_inside_sphere) {
  ray_t r = ray(point(0, 0, 0), vector(0, 0, 1));
  sphere_t s = sphere(0);
  intersection_list_t ilist = sphere_intersect(s, r);

  ASSERT_EQ(ilist.size, 2, "%d");
  ASSERT_TRUE(req(ilist.items[0].t, -1.0));
  ASSERT_TRUE(req(ilist.items[1].t, 1.0));
  free_intersection_list(ilist);
}

TEST_CASE(ray_behind_sphere) {
  ray_t r = ray(point(0, 0, 5), vector(0, 0, 1));
  sphere_t s = sphere(0);
  intersection_list_t ilist = sphere_intersect(s, r);

  ASSERT_EQ(ilist.size, 2, "%d");
  ASSERT_TRUE(req(ilist.items[0].t, -6.0));
  ASSERT_TRUE(req(ilist.items[1].t, -4.0));
  free_intersection_list(ilist);
}


TEST_CASE(intersect_sets_object) {
  ray_t r = ray(point(0, 0, -5), vector(0, 0, 1));
  sphere_t s = sphere(123);
  intersection_list_t ilist = sphere_intersect(s, r);

  ASSERT_EQ(ilist.size, 2, "%d");
  ASSERT_TRUE(req(ilist.items[0].type, TYPE_SPHERE));
  ASSERT_TRUE(req(ilist.items[0].id, 123));
  ASSERT_TRUE(req(ilist.items[1].type, TYPE_SPHERE));
  ASSERT_TRUE(req(ilist.items[1].id, 123));
  free_intersection_list(ilist);
}

TEST_CASE(sphere_default_transformation) {
  sphere_t s = sphere(1);

  ASSERT_TRUE(matrix4_eq(s.transform, matrix4_identity()));
}

TEST_CASE(sphere_set_the_transform) {
  sphere_t s = sphere(1);
  matrix4_t t = translation(2, 3, 4);
  sphere_set_transform(&s, t);

  ASSERT_TRUE(matrix4_eq(s.transform, t));
  ASSERT_TRUE(matrix4_eq(s.inverse_transform, matrix4_inverse(t)));
}

TEST_CASE(intersecting_a_scaled_sphere) {
  ray_t r = ray(point(0, 0, -5), vector(0, 0, 1));
  sphere_t s = sphere(1);
  sphere_set_transform(&s, scaling(2, 2, 2));

  intersection_list_t ilist = sphere_intersect(s, r);
  ASSERT_EQ(ilist.size, 2, "%d");
  ASSERT_TRUE(req(ilist.items[0].t, 3));
  ASSERT_TRUE(req(ilist.items[1].t, 7));
  free_intersection_list(ilist);
}

TEST_CASE(intersecting_a_translated_sphere) {
  ray_t r = ray(point(0, 0, -5), vector(0, 0, 1));
  sphere_t s = sphere(1);
  sphere_set_transform(&s, translation(5, 0, 0));

  intersection_list_t ilist = sphere_intersect(s, r);
  ASSERT_EQ(ilist.size, 0, "%d");
  free_intersection_list(ilist);
}
