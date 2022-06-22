
#include "labrat.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "color.h"
#include "util.h"

color_t color(double r, double g, double b) {
	color_t c;
	c.r = r;
	c.g = g;
	c.b = b;
	return c;
}

bool color_eq(color_t a, color_t b) {
	return req(a.r, b.r) && req(a.g, b.g) && req(a.b, b.b);
}

color_t color_add(color_t a, color_t b) {
	return color(
			a.r + b.r,
			a.g + b.g,
			a.b + b.b
			);
}

color_t color_sub(color_t a, color_t b) {
	return color(
			a.r - b.r,
			a.g - b.g,
			a.b - b.b
			);
}

color_t color_muls(color_t a, double s) {
	return color(
			a.r * s,
			a.g * s,
			a.b * s
			);
}

color_t color_mul(color_t a, color_t b) {
	return color(
			a.r * b.r,
			a.g * b.g,
			a.b * b.b
			);
}

TEST_CASE(colors_are_rgb_tuples) {
	color_t c = color(-0.5, 0.4, 1.7);
	ASSERT_TRUE(req(c.r, -0.5));
	ASSERT_TRUE(req(c.g, 0.4));
	ASSERT_TRUE(req(c.b, 1.7));
}

TEST_CASE(adding_colors) {
	color_t c1 = color(0.9, 0.6, 0.75);
	color_t c2 = color(0.7, 0.1, 0.25);
	ASSERT_TRUE(color_eq(color_add(c1, c2), color(1.6, 0.7, 1.0)));
}

TEST_CASE(subtracting_colors) {
	color_t c1 = color(0.9, 0.6, 0.75);
	color_t c2 = color(0.7, 0.1, 0.25);
	ASSERT_TRUE(color_eq(color_sub(c1, c2), color(0.2, 0.5, 0.5)));
}

TEST_CASE(scale_color) {
	color_t c = color(0.2, 0.3, 0.4);
	ASSERT_TRUE(color_eq(color_muls(c, 2), color(0.4, 0.6, 0.8)));
}

TEST_CASE(multiply_colors) {
	color_t c1 = color(1, 0.2, 0.4);
	color_t c2 = color(0.9, 1, 0.1);
	ASSERT_TRUE(color_eq(color_mul(c1, c2), color(0.9, 0.2, 0.04)));
}
