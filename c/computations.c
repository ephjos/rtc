
#include "labrat.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "rtc.h"

computations_t prepare_computations(intersection_t i, ray_t r) {
	computations_t c;

	c.t = i.t;
	c.s = i.s;
	c.point = ray_position(r, c.t);
	c.eyev = vec4_neg(r.direction);
	c.normalv = c.s.normal_at(c.s, c.point);
	if (vec4_dot(c.normalv, c.eyev) < 0) {
		c.inside = true;
		c.normalv = vec4_neg(c.normalv);
	} else {
		c.inside = false;
	}

	c.over_point = vec4_add(c.point, vec4_muls(c.normalv, EPSILON));

	return c;
}


TEST_CASE(preparing_the_state_of_an_intersection) {
	ray_t r = ray(point(0, 0, -5), vector(0, 0, 1));
	shape_t s = sphere();
	intersection_t i = {
		.t = 4,
		.s = s,
	};
	computations_t c = prepare_computations(i, r);
	ASSERT_TRUE(req(c.t, i.t));
	ASSERT_TRUE(shape_eq(c.s, i.s));
	ASSERT_TRUE(vec4_eq(c.point, point(0, 0, -1)));
	ASSERT_TRUE(vec4_eq(c.eyev, vector(0, 0, -1)));
	ASSERT_TRUE(vec4_eq(c.normalv, vector(0, 0, -1)));

	shape_free(s);
}

TEST_CASE(the_hit_with_intersection_outside) {
	ray_t r = ray(point(0, 0, -5), vector(0, 0, 1));
	shape_t s = sphere();
	intersection_t i = {
		.t = 4,
		.s = s,
	};
	computations_t c = prepare_computations(i, r);
	ASSERT_FALSE(c.inside);

	shape_free(s);
}

TEST_CASE(the_hit_with_intersection_inside) {
	ray_t r = ray(point(0, 0, 0), vector(0, 0, 1));
	shape_t s = sphere();
	intersection_t i = {
		.t = 1,
		.s = s,
	};
	computations_t c = prepare_computations(i, r);
	ASSERT_TRUE(req(c.t, i.t));
	ASSERT_TRUE(shape_eq(c.s, i.s));
	ASSERT_TRUE(vec4_eq(c.point, point(0, 0, 1)));
	ASSERT_TRUE(vec4_eq(c.eyev, vector(0, 0, -1)));
	ASSERT_TRUE(vec4_eq(c.normalv, vector(0, 0, -1)));
	ASSERT_TRUE(c.inside);

	shape_free(s);
}

TEST_CASE(the_hit_should_offset_the_point) {
	ray_t r = ray(point(0, 0, -5), vector(0, 0, 1));
	shape_t s = sphere();
	shape_set_transform(&s, translation(0, 0, 1));

	intersection_t i = {
		.t = 5,
		.s = s,
	};
	computations_t comps = prepare_computations(i, r);
	ASSERT_TRUE(comps.over_point.z < (-EPSILON / 2));
	ASSERT_TRUE(comps.point.z > comps.over_point.z);

	shape_free(s);
}

