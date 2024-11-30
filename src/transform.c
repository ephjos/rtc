#include "rtc.h"

void translation(f64 x, f64 y, f64 z, matrix4 out)
{
  const matrix4 temp = {
    1, 0, 0, x,
    0, 1, 0, y,
    0, 0, 1, z,
    0, 0, 0, 1,
  };

  memcpy(out, temp, sizeof(matrix4));
}

void scaling(f64 x, f64 y, f64 z, matrix4 out)
{
  const matrix4 temp = {
    x, 0, 0, 0,
    0, y, 0, 0,
    0, 0, z, 0,
    0, 0, 0, 1,
  };

  memcpy(out, temp, sizeof(matrix4));
}

void rotation_x(f64 r, matrix4 out)
{
  const matrix4 temp = {
    1, 0,        0,       0,
    0, COS(r), -SIN(r), 0,
    0, SIN(r),  COS(r), 0,
    0, 0,        0,       1,
  };

  memcpy(out, temp, sizeof(matrix4));
}
void rotation_y(f64 r, matrix4 out)
{
  const matrix4 temp = {
     COS(r), 0, SIN(r), 0,
     0,       1, 0,       0,
    -SIN(r), 0, COS(r), 0,
     0,       0, 0,       1,
  };

  memcpy(out, temp, sizeof(matrix4));
}
void rotation_z(f64 r, matrix4 out)
{
  const matrix4 temp = {
    COS(r), -SIN(r), 0, 0,
    SIN(r),  COS(r), 0, 0,
    0,        0,       1, 0,
    0,        0,       0, 1,
  };

  memcpy(out, temp, sizeof(matrix4));
}

void shearing(f64 xy, f64 xz, f64 yx, f64 yz, f64 zx, f64 zy, matrix4 out)
{
  const matrix4 temp = {
    1,  xy, xz, 0,
    yx, 1,  yz, 0,
    zx, zy, 1,  0,
    0,  0,  0,  1,
  };

  memcpy(out, temp, sizeof(matrix4));
}

void view_transform(vec4 from, vec4 to, vec4 up, matrix4 out)
{
  vec4 forward = {0};
  vec4_sub(to, from, forward);
  vec4_norm(forward, forward);

  vec4 up_n = {0};
  vec4_norm(up, up_n);

  vec4 left = {0};
  vec4_cross(forward, up_n, left);

  vec4 true_up = {0};
  vec4_cross(left, forward, true_up);

  matrix4 orientation = {
     left[0],     left[1],     left[2],    0,
     true_up[0],  true_up[1],  true_up[2], 0,
    -forward[0], -forward[1], -forward[2], 0,
     0,           0,           0,          1,
  };

  matrix4 from_translation = {0};
  translation(-from[0], -from[1], -from[2], from_translation);

  matrix4_mul(orientation, from_translation, out);
}

