
#include "labrat.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "rtc.h"

point_light_t point_light(vec4_t position, color_t intensity) {
	point_light_t pl;
	pl.position = position;
	pl.intensity = intensity;
	return pl;
}

bool point_light_eq(point_light_t a, point_light_t b) {
	return vec4_eq(a.position, b.position) && color_eq(a.intensity, b.intensity);
}

TEST_CASE(point_light_has_postion_and_intensity) {
	vec4_t position = point(0, 0, 0);
	color_t intensity = color(1, 1, 1);
	point_light_t pl = point_light(position, intensity);

	ASSERT_TRUE(vec4_eq(pl.position, position));
	ASSERT_TRUE(color_eq(pl.intensity, intensity));
}
