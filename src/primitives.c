#include "rtc.h"

void v3_print(const v3 a)
{
  printf("[%0.5f, %0.5f, %0.5f]\n", 
      (f64)a[0], (f64)a[1], (f64)a[2]);
}

b32 v3_eq(const v3 a, const v3 b)
{
  return req(a[0], b[0]) &&
         req(a[1], b[1]) &&
         req(a[2], b[2]);
}

void v3_mul(const v3 a, const v3 b, v3 out)
{
  out[0] = a[0] * b[0];
  out[1] = a[1] * b[1];
  out[2] = a[2] * b[2];
}

// -----------------------------------------------------------------------------
  
const v4 ZERO_VEC = vector_init(0,0,0);

void v4_print(const v4 a)
{
  printf("[%0.5f, %0.5f, %0.5f, %0.5f]\n", 
      (f64)a[0], (f64)a[1], (f64)a[2], (f64)a[3]);
}

b32 is_point(const v4 a)
{
  return req(a[3], 1.0);
}

b32 is_vector(const v4 a)
{
  return req(a[3], 0.0);
}

f64 v4_mag(const v4 a)
{
  f64 dot = v4_dot(a, a);
  return (f64)sqrt(dot);
}

void v4_neg(const v4 a, v4 out)
{
  out[0] = -a[0];
  out[1] = -a[1];
  out[2] = -a[2];
  out[3] = -a[3];
}

void v4_norm(const v4 a, v4 out)
{
  f64 mag = v4_mag(a);
  v4_scale(a, 1.0/mag, out);
}

b32 v4_eq(const v4 a, const v4 b)
{
  return req(a[0], b[0]) &&
         req(a[1], b[1]) &&
         req(a[2], b[2]) &&
         req(a[3], b[3]);
}

void v4_mul(const v4 a, const v4 b, v4 out)
{
  out[0] = a[0] * b[0];
  out[1] = a[1] * b[1];
  out[2] = a[2] * b[2];
  out[3] = a[3] * b[3];
}

void v4_div(const v4 a, const v4 b, v4 out)
{
  out[0] = a[0] / b[0];
  out[1] = a[1] / b[1];
  out[2] = a[2] / b[2];
  out[3] = a[3] / b[3];
}

void v4_cross(const v4 a, const v4 b, v4 out)
{
  out[0] = a[1] * b[2] - a[2] * b[1];
  out[1] = a[2] * b[0] - a[0] * b[2];
  out[2] = a[0] * b[1] - a[1] * b[0];
  out[3] = 0;
}

void v4_reflect(const v4 v, const v4 n, v4 out)
{
  f64 scale = 2 * v4_dot(v, n);

  v4 scaled_normal = {0};
  v4_scale(n, scale, scaled_normal);

  //v4_sub(v, scaled_normal, out);
  out[0] = v[0] - scaled_normal[0];
  out[1] = v[1] - scaled_normal[1];
  out[2] = v[2] - scaled_normal[2];
  out[3] = v[3] - scaled_normal[3];
}

/*
f64 v4_dot(const v4 a, const v4 b)
{
  return (a[0]*b[0]) +
         (a[1]*b[1]) +
         (a[2]*b[2]) +
         (a[3]*b[3]);
}
*/


