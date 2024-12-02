#include "rtc.h"

void translation(f64 x, f64 y, f64 z, m4 out)
{
  const m4 temp = {
    1, 0, 0, x,
    0, 1, 0, y,
    0, 0, 1, z,
    0, 0, 0, 1,
  };

  memcpy(out, temp, sizeof(m4));
}

void scaling(f64 x, f64 y, f64 z, m4 out)
{
  const m4 temp = {
    x, 0, 0, 0,
    0, y, 0, 0,
    0, 0, z, 0,
    0, 0, 0, 1,
  };

  memcpy(out, temp, sizeof(m4));
}

void rotation_x(f64 r, m4 out)
{
  const m4 temp = {
    1, 0,        0,       0,
    0, COS(r), -SIN(r), 0,
    0, SIN(r),  COS(r), 0,
    0, 0,        0,       1,
  };

  memcpy(out, temp, sizeof(m4));
}
void rotation_y(f64 r, m4 out)
{
  const m4 temp = {
     COS(r), 0, SIN(r), 0,
     0,       1, 0,       0,
    -SIN(r), 0, COS(r), 0,
     0,       0, 0,       1,
  };

  memcpy(out, temp, sizeof(m4));
}
void rotation_z(f64 r, m4 out)
{
  const m4 temp = {
    COS(r), -SIN(r), 0, 0,
    SIN(r),  COS(r), 0, 0,
    0,        0,       1, 0,
    0,        0,       0, 1,
  };

  memcpy(out, temp, sizeof(m4));
}

void shearing(f64 xy, f64 xz, f64 yx, f64 yz, f64 zx, f64 zy, m4 out)
{
  const m4 temp = {
    1,  xy, xz, 0,
    yx, 1,  yz, 0,
    zx, zy, 1,  0,
    0,  0,  0,  1,
  };

  memcpy(out, temp, sizeof(m4));
}

void view_transform(v4 from, v4 to, v4 up, m4 out)
{
  v4 forward = {0};
  v4_sub(to, from, forward);
  v4_norm(forward, forward);

  v4 up_n = {0};
  v4_norm(up, up_n);

  v4 left = {0};
  v4_cross(forward, up_n, left);

  v4 true_up = {0};
  v4_cross(left, forward, true_up);

  m4 orientation = {
     left[0],     left[1],     left[2],    0,
     true_up[0],  true_up[1],  true_up[2], 0,
    -forward[0], -forward[1], -forward[2], 0,
     0,           0,           0,          1,
  };

  m4 from_translation = {0};
  translation(-from[0], -from[1], -from[2], from_translation);

  m4_mul(orientation, from_translation, out);
}

