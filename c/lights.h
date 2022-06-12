#pragma once

#include "canvas.h"
#include "vec4.h"

typedef struct point_light_t {
	vec4_t position;
	color_t intensity;
} point_light_t;

point_light_t point_light(vec4_t position, color_t intensity);

