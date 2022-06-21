#pragma once

#include "matrix4.h"

matrix4_t translation(float x, float y, float z);
matrix4_t scaling(float x, float y, float z);
matrix4_t rotate_x(float r);
matrix4_t rotate_y(float r);
matrix4_t rotate_z(float r);
matrix4_t shearing(float a, float b, float c, float d, float e, float f);

matrix4_t transform(matrix4_t *ms, int n);

matrix4_t view_transform(vec4_t from, vec4_t to, vec4_t up);

void transform_demo();
