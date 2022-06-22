#pragma once

#include "matrix4.h"

matrix4_t translation(double x, double y, double z);
matrix4_t scaling(double x, double y, double z);
matrix4_t rotate_x(double r);
matrix4_t rotate_y(double r);
matrix4_t rotate_z(double r);
matrix4_t shearing(double a, double b, double c, double d, double e, double f);

matrix4_t transform(matrix4_t *ms, int n);

matrix4_t view_transform(vec4_t from, vec4_t to, vec4_t up);

void transform_demo();
