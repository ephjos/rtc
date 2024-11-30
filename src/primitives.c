#include "rtc.h"
  
const vec4 ZERO_VEC = vec4_init(0,0,0);

void vec4_print(const vec4 a)
{
  printf("[%0.5f, %0.5f, %0.5f, %0.5f]\n", 
      (f64)a[0], (f64)a[1], (f64)a[2], (f64)a[3]);
}

b32 is_point4(const vec4 a)
{
  return req(a[3], 1.0);
}

b32 is_vec4(const vec4 a)
{
  return req(a[3], 0.0);
}

f64 vec4_mag(const vec4 a)
{
  f64 dot = vec4_dot(a, a);
  return (f64)sqrt(dot);
}

void vec4_neg(const vec4 a, vec4 out)
{
  vec4_sub(ZERO_VEC, a, out);
}

void vec4_norm(const vec4 a, vec4 out)
{
  f64 mag = vec4_mag(a);
  vec4_scale(a, 1.0/mag, out);
}

b32 vec4_eq(const vec4 a, const vec4 b)
{
  return req(a[0], b[0]) &&
         req(a[1], b[1]) &&
         req(a[2], b[2]) &&
         req(a[3], b[3]);
}

void vec4_add(const vec4 a, const vec4 b, vec4 out)
{
  /*
  __m128 r = _mm_loadu_ps(a);
  __m128 s = _mm_loadu_ps(b);

  __m128 vec4_res = _mm_add_ps(r, s);

  _mm_storeu_ps(out, vec4_res);
  */
  vec4 temp = {
    a[0] + b[0],
    a[1] + b[1],
    a[2] + b[2],
    a[3] + b[3],
  };
  memcpy(out, temp, sizeof(vec4));
}

void vec4_sub(const vec4 a, const vec4 b, vec4 out)
{
  /*
  __m128 r = _mm_loadu_ps(a);
  __m128 s = _mm_loadu_ps(b);

  __m128 vec4_res = _mm_sub_ps(r, s);

  _mm_storeu_ps(out, vec4_res);
  */
  vec4 temp = {
    a[0] - b[0],
    a[1] - b[1],
    a[2] - b[2],
    a[3] - b[3],
  };
  memcpy(out, temp, sizeof(vec4));
}

void vec4_mul(const vec4 a, const vec4 b, vec4 out)
{
  /*
  __m128 r = _mm_loadu_ps(a);
  __m128 s = _mm_loadu_ps(b);

  __m128 vec4_res = _mm_mul_ps(r, s);

  _mm_storeu_ps(out, vec4_res);
  */
  vec4 temp = {
    a[0] * b[0],
    a[1] * b[1],
    a[2] * b[2],
    a[3] * b[3],
  };
  memcpy(out, temp, sizeof(vec4));
}

void vec4_div(const vec4 a, const vec4 b, vec4 out)
{
  /*
  // NOTE: could just use mul?
  __m128 r = _mm_loadu_ps(a);
  __m128 s = _mm_loadu_ps(b);

  __m128 vec4_res = _mm_div_ps(r, s);

  _mm_storeu_ps(out, vec4_res);
  */
  vec4 temp = {
    a[0] / b[0],
    a[1] / b[1],
    a[2] / b[2],
    a[3] / b[3],
  };
  memcpy(out, temp, sizeof(vec4));
}

void vec4_cross(const vec4 a, const vec4 b, vec4 out)
{
  vec4 temp = {
    a[1] * b[2] - a[2] * b[1],
    a[2] * b[0] - a[0] * b[2],
    a[0] * b[1] - a[1] * b[0],
    0,
  };

  memcpy(out, temp, sizeof(vec4));
}

void vec4_reflect(const vec4 v, const vec4 n, vec4 out)
{
  f64 scale = 2 * vec4_dot(v, n);

  vec4 scaled_normal = {0};
  vec4_scale(n, scale, scaled_normal);

  vec4_sub(v, scaled_normal, out);
}

f64 vec4_dot(const vec4 a, const vec4 b)
{
  vec4 out = {0};
  vec4_mul(a, b, out);
  return out[0] + out[1] + out[2] + out[3];
}

void vec4_scale(const vec4 a, f64 b, vec4 out)
{
  vec4_mul(a, (vec4){b, b, b, b}, out);
}


