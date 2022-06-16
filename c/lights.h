#pragma once

#include "canvas.h"
#include "vec4.h"

typedef struct point_light_t {
	vec4_t position;
	color_t intensity;
} point_light_t;

point_light_t point_light(vec4_t position, color_t intensity);
bool point_light_eq(point_light_t a, point_light_t b);

