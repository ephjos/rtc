#pragma once

#include "matrix4.h"
#include "vec4.h"

typedef struct ray_t {
  vec4_t origin;
  vec4_t direction;
} ray_t;

ray_t ray(vec4_t origin, vec4_t direction);
vec4_t ray_position(ray_t r, double t);
ray_t ray_transform(ray_t r, matrix4_t m);

void ray_demo();
