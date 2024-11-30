#include "rtc.h"


const matrix4 IDENTITY = {
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1,
};

void matrix4_print(const matrix4 a)
{
  printf("[\n");
  printf(" %0.2f, %0.2f, %0.2f, %0.2f\n", (double)a[0],  (double)a[1],  (double)a[2],  (double)a[3]);
  printf(" %0.2f, %0.2f, %0.2f, %0.2f\n", (double)a[4],  (double)a[5],  (double)a[6],  (double)a[6]);
  printf(" %0.2f, %0.2f, %0.2f, %0.2f\n", (double)a[8],  (double)a[9],  (double)a[10], (double)a[11]);
  printf(" %0.2f, %0.2f, %0.2f, %0.2f\n", (double)a[12], (double)a[13], (double)a[14], (double)a[15]);
  printf("]\n");
}

b32 matrix4_eq(const matrix4 A, const matrix4 B)
{ 
  return req(A[0], B[0]) && 
         req(A[1], B[1]) && 
         req(A[2], B[2]) && 
         req(A[3], B[3]) && 
         req(A[4], B[4]) && 
         req(A[5], B[5]) && 
         req(A[6], B[6]) && 
         req(A[7], B[7]) && 
         req(A[8], B[8]) && 
         req(A[9], B[9]) && 
         req(A[10], B[10]) && 
         req(A[11], B[11]) && 
         req(A[12], B[12]) && 
         req(A[13], B[13]) && 
         req(A[14], B[14]) && 
         req(A[15], B[15]);
}

void matrix4_mul(const matrix4 A, const matrix4 B, matrix4 out)
{
  /* Simple
  for (u32 i = 0; i < 4; i++) {
    for (u32 j = 0; j < 4; j++) {
      vec4 b = { B[0][j],B[1][j],B[2][j],B[3][j] };
      out[i][j] = vec4_dot(A[i], b);
    }
  }
  */

  // META
  vec4 a0 = { A[0 _ 0], A[0 _ 1], A[0 _ 2], A[0 _ 3] };
  vec4 a1 = { A[1 _ 0], A[1 _ 1], A[1 _ 2], A[1 _ 3] };
  vec4 a2 = { A[2 _ 0], A[2 _ 1], A[2 _ 2], A[2 _ 3] };
  vec4 a3 = { A[3 _ 0], A[3 _ 1], A[3 _ 2], A[3 _ 3] };

  vec4 b0 = { B[0 _ 0], B[1 _ 0], B[2 _ 0], B[3 _ 0] };
  vec4 b1 = { B[0 _ 1], B[1 _ 1], B[2 _ 1], B[3 _ 1] };
  vec4 b2 = { B[0 _ 2], B[1 _ 2], B[2 _ 2], B[3 _ 2] };
  vec4 b3 = { B[0 _ 3], B[1 _ 3], B[2 _ 3], B[3 _ 3] };

  const matrix4 temp = {
    vec4_dot(a0, b0), vec4_dot(a0, b1), vec4_dot(a0, b2), vec4_dot(a0, b3),
    vec4_dot(a1, b0), vec4_dot(a1, b1), vec4_dot(a1, b2), vec4_dot(a1, b3),
    vec4_dot(a2, b0), vec4_dot(a2, b1), vec4_dot(a2, b2), vec4_dot(a2, b3),
    vec4_dot(a3, b0), vec4_dot(a3, b1), vec4_dot(a3, b2), vec4_dot(a3, b3),
  };
  memcpy(out, temp, sizeof(matrix4));
}

void matrix4_mulv(const matrix4 A, const vec4 b, vec4 out)
{

  // META

  vec4 a0 = { A[0 _ 0], A[0 _ 1], A[0 _ 2], A[0 _ 3] };
  vec4 a1 = { A[1 _ 0], A[1 _ 1], A[1 _ 2], A[1 _ 3] };
  vec4 a2 = { A[2 _ 0], A[2 _ 1], A[2 _ 2], A[2 _ 3] };
  vec4 a3 = { A[3 _ 0], A[3 _ 1], A[3 _ 2], A[3 _ 3] };

  vec4 temp = {
    vec4_dot(a0, b),
    vec4_dot(a1, b),
    vec4_dot(a2, b),
    vec4_dot(a3, b),
  };
  memcpy(out, temp, sizeof(vec4));
}

void matrix4_transpose(const matrix4 A, matrix4 out)
{
  // META
  const matrix4 temp = {
    A[0 _ 0], A[1 _ 0], A[2 _ 0], A[3 _ 0],
    A[0 _ 1], A[1 _ 1], A[2 _ 1], A[3 _ 1],
    A[0 _ 2], A[1 _ 2], A[2 _ 2], A[3 _ 2],
    A[0 _ 3], A[1 _ 3], A[2 _ 3], A[3 _ 3],
  };
  memcpy(out, temp, sizeof(matrix4));
}

f64 matrix4_det(const matrix4 A)
{
  // Hand-unwound matrix determinant. Avoids RTC prescribed variable sized
  // matrices and function (e.g. cofactor) calls.
  f64 a = A[0];
  f64 b = A[1];
  f64 c = A[2];
  f64 d = A[3];
  f64 e = A[4];
  f64 f = A[5];
  f64 g = A[6];
  f64 h = A[7];
  f64 i = A[8];
  f64 j = A[9];
  f64 k = A[10];
  f64 l = A[11];
  f64 m = A[12];
  f64 n = A[13];
  f64 o = A[14];
  f64 p = A[15];

  f64 ca = f * (k * p - l * o) - g * (j * p - l * n) + h * (j * o - k * n);
  f64 cb = -e * (k * p - l * o) + g * (i * p - l * m) - h * (i * o - k * m);
  f64 cc = e * (j * p - l * n) - f * (i * p - l * m) + h * (i * n - j * m);
  f64 cd = -e * (j * o - k * n) + f * (i * o - k * m) - g * (i * n - j * m);

  return a * ca + b * cb + c * cc + d * cd;
}

void matrix4_inverse(const matrix4 A, matrix4 out)
{
  // Hand-unwound matrix inversion. Avoids RTC prescribed variable sized
  // matrices and function (e.g. cofactor) calls.
  //
  // Returns without modifying out if uninvertible.
  f64 a = A[0];
  f64 b = A[1];
  f64 c = A[2];
  f64 d = A[3];
  f64 e = A[4];
  f64 f = A[5];
  f64 g = A[6];
  f64 h = A[7];
  f64 i = A[8];
  f64 j = A[9];
  f64 k = A[10];
  f64 l = A[11];
  f64 m = A[12];
  f64 n = A[13];
  f64 o = A[14];
  f64 p = A[15];

  f64 ca = f * (k * p - l * o) - g * (j * p - l * n) + h * (j * o - k * n);
  f64 cb = -e * (k * p - l * o) + g * (i * p - l * m) - h * (i * o - k * m);
  f64 cc = e * (j * p - l * n) - f * (i * p - l * m) + h * (i * n - j * m);
  f64 cd = -e * (j * o - k * n) + f * (i * o - k * m) - g * (i * n - j * m);

  f64 det = a * ca + b * cb + c * cc + d * cd;
  if (det == 0) {
    // Can't invert
    return;
  }

  f64 ce = -b * (k * p - l * o) + c * (j * p - l * n) - d * (j * o - k * n);
  f64 cf = a * (k * p - l * o) - c * (i * p - l * m) + d * (i * o - k * m);
  f64 cg = -a * (j * p - l * n) + b * (i * p - l * m) - d * (i * n - j * m);
  f64 ch = a * (j * o - k * n) - b * (i * o - k * m) + c * (i * n - j * m);

  f64 ci = b * (g * p - h * o) - c * (f * p - h * n) + d * (f * o - g * n);
  f64 cj = -a * (g * p - h * o) + c * (e * p - h * m) - d * (e * o - g * m);
  f64 ck = a * (f * p - h * n) - b * (e * p - h * m) + d * (e * n - f * m);
  f64 cl = -a * (f * o - g * n) + b * (e * o - g * m) - c * (e * n - f * m);

  f64 cm = -b * (g * l - h * k) + c * (f * l - h * j) - d * (f * k - g * j);
  f64 cn = a * (g * l - h * k) - c * (e * l - h * i) + d * (e * k - g * i);
  f64 co = -a * (f * l - h * j) + b * (e * l - h * i) - d * (e * j - f * i);
  f64 cp = a * (f * k - g * j) - b * (e * k - g * i) + c * (e * j - f * i);

  const matrix4 temp = {
    ca / det, ce / det, ci / det, cm / det,
    cb / det, cf / det, cj / det, cn / det,
    cc / det, cg / det, ck / det, co / det,
    cd / det, ch / det, cl / det, cp / det,
  };
  memcpy(out, temp, sizeof(matrix4));
}

