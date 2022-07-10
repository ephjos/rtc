
#include "labrat.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "rtc.h"

// Shape
bool shape_eq(shape_t a, shape_t b) {
	return matrix4_eq(a.transform, b.transform) &&
		matrix4_eq(a.inverse_transform, b.inverse_transform) &&
		material_eq(a.material, b.material);
}

void shape_set_transform(shape_t *s, matrix4_t t) {
	s->transform = t;
	s->inverse_transform = matrix4_inverse(t);
}

void shape_free(shape_t s) {
	free(s.instance);
}

