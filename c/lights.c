
#include "labrat.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "canvas.h"
#include "error.h"
#include "lights.h"
#include "matrix4.h"
#include "ray.h"
#include "sphere.h"
#include "transform.h"
#include "util.h"
#include "vec4.h"

point_light_t point_light(vec4_t position, color_t intensity) {
	point_light_t pl;
	pl.position = position;
	pl.intensity = intensity;
	return pl;
}

TEST_CASE(point_light_has_postion_and_intensity) {
	vec4_t position = point(0, 0, 0);
	color_t intensity = color(1, 1, 1);
	point_light_t pl = point_light(position, intensity);

	ASSERT_TRUE(vec4_eq(pl.position, position));
	ASSERT_TRUE(color_eq(pl.intensity, intensity));
}
