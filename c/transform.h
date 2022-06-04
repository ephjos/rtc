#pragma once

#include "matrix.h"

matrix_t translation(float x, float y, float z);
matrix_t scaling(float x, float y, float z);
matrix_t rotate_x(float r);
matrix_t rotate_y(float r);
matrix_t rotate_z(float r);
matrix_t shearing(float a, float b, float c, float d, float e, float f);

matrix_t transform(matrix_t *ms, int n);

void transform_demo();
