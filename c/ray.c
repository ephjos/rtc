
#include "labrat.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "canvas.h"
#include "error.h"
#include "matrix4.h"
#include "ray.h"
#include "shape.h"
#include "transform.h"
#include "util.h"
#include "vec4.h"

ray_t ray(vec4_t origin, vec4_t direction) {
  ray_t r;
  r.origin = origin;
  r.direction = direction;
  return r;
}

vec4_t ray_position(ray_t r, float t) {
  return vec4_add(r.origin, vec4_muls(r.direction, t));
}

ray_t ray_transform(ray_t r, matrix4_t m) {
  ray_t res;
  res.origin = matrix4_mul_vec4(m, r.origin);
  res.direction = matrix4_mul_vec4(m, r.direction);
  return res;
}

TEST_CASE(creating_and_querying_ray) {
  vec4_t origin = point(1, 2, 3);
  vec4_t direction = vector(4, 5, 6);

  ray_t r = ray(origin, direction);
  ASSERT_TRUE(vec4_eq(r.origin, origin));
  ASSERT_TRUE(vec4_eq(r.direction, direction));
}

TEST_CASE(computing_point_from_distance) {
  ray_t r = ray(point(2, 3, 4), vector(1, 0, 0));

  ASSERT_TRUE(vec4_eq(ray_position(r, 0), point(2, 3, 4)));
  ASSERT_TRUE(vec4_eq(ray_position(r, 1), point(3, 3, 4)));
  ASSERT_TRUE(vec4_eq(ray_position(r, -1), point(1, 3, 4)));
  ASSERT_TRUE(vec4_eq(ray_position(r, 2.5), point(4.5, 3, 4)));
}

TEST_CASE(translating_a_ray) {
  ray_t r = ray(point(1, 2, 3), vector(0, 1, 0));
  matrix4_t m = translation(3, 4, 5);

  ray_t r2 = ray_transform(r, m);
  ASSERT_TRUE(vec4_eq(r2.origin, point(4, 6, 8)));
  ASSERT_TRUE(vec4_eq(r2.direction, vector(0, 1, 0)));
}

TEST_CASE(scaling_a_ray) {
  ray_t r = ray(point(1, 2, 3), vector(0, 1, 0));
  matrix4_t m = scaling(2, 3, 4);

  ray_t r2 = ray_transform(r, m);
  ASSERT_TRUE(vec4_eq(r2.origin, point(2, 6, 12)));
  ASSERT_TRUE(vec4_eq(r2.direction, vector(0, 3, 0)));
}

void ray_demo() {
  vec4_t from = point(0, 0, 0);
  shape_t s = sphere(0);
  matrix4_t st[] = {
    translation(0, 0, 7),
    scaling(1, 2, 1),
    rotate_z(0.5),
  };
  shape_set_transform(&s, transform(st, 3));

  color_t red = color(1, 0, 0);
  const float WALL_Z = 14;
  const float WALL_SIZE = 10;
  const float SIZE = 128;
  canvas_t c = canvas(SIZE, SIZE);
  const float PIXEL_SIZE = WALL_SIZE / SIZE;

  for (int i = 0; i < SIZE; i++) {
    float y = (WALL_SIZE/2) - PIXEL_SIZE * i;
    for (int j = 0; j < SIZE; j++) {
      float x = (-WALL_SIZE/2) + PIXEL_SIZE * j;
      ray_t r = ray(from, vec4_sub(point(x, y, WALL_Z), from));
      intersection_list_t ilist = intersection_list();
      sphere_intersect(s, r, &ilist);
      if (ilist.hit != NULL) {
        canvas_write(&c, j, i, red);
      }
      free_intersection_list(ilist);
    }
  }

  shape_free(s);

  canvas_save(c, "./ray_demo.ppm");
  canvas_free(c);
}
