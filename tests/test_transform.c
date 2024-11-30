
#include "tests.h"

void test_transform()
{
  TESTS();

  TEST {
      // Multiplying by a translation matrix
      vec4 p = point4_init(-3, 4, 5);

      matrix4 T = {0};
      translation(5, -3, 2, T);

      vec4 actual = {0};
      matrix4_mulv(T, p, actual);

      vec4 expected = point4_init(2, 1, 7);

      assert(vec4_eq(actual, expected));
  }

  TEST {
      // Multiplying by inverse translation matrix
      vec4 p = point4_init(-3, 4, 5);

      matrix4 T = {0};
      translation(5, -3, 2, T);

      matrix4 TI = {0};
      matrix4_inverse(T, TI);

      vec4 actual = {0};
      matrix4_mulv(TI, p, actual);

      vec4 expected = point4_init(-8, 7, 3);

      assert(vec4_eq(actual, expected));
  }

  TEST {
      // Can't translate vectors
      vec4 v = vec4_init(-3, 4, 5);

      matrix4 T = {0};
      translation(5, -3, 2, T);

      vec4 actual = {0};
      matrix4_mulv(T, v, actual);

      assert(vec4_eq(actual, v));
  }

  TEST {
      // Scaling a point
      vec4 p = point4_init(-4, 6, 8);

      matrix4 T = {0};
      scaling(2, 3, 4, T);

      vec4 actual = {0};
      matrix4_mulv(T, p, actual);

      vec4 expected = point4_init(-8, 18, 32);

      assert(vec4_eq(actual, expected));
  }

  TEST {
      // Scaling a vector
      vec4 v = vec4_init(-4, 6, 8);

      matrix4 T = {0};
      scaling(2, 3, 4, T);

      vec4 actual = {0};
      matrix4_mulv(T, v, actual);

      vec4 expected = vec4_init(-8, 18, 32);

      assert(vec4_eq(actual, expected));
  }

  TEST {
      // Inverse scaling a vector
      vec4 v = vec4_init(-4, 6, 8);

      matrix4 T = {0};
      scaling(2, 3, 4, T);

      matrix4 TI = {0};
      matrix4_inverse(T, TI);

      vec4 actual = {0};
      matrix4_mulv(TI, v, actual);

      vec4 expected = vec4_init(-2, 2, 2);

      assert(vec4_eq(actual, expected));
  }

  TEST {
      // Reflection is scaling by a negative value
      vec4 v = vec4_init(2, 3, 4);

      matrix4 T = {0};
      scaling(-1, 1, 1, T);

      vec4 actual = {0};
      matrix4_mulv(T, v, actual);

      vec4 expected = vec4_init(-2, 3, 4);

      assert(vec4_eq(actual, expected));
  }

  TEST {
      // Rotating a point around the X axis
      vec4 p = point4_init(0, 1, 0);

      matrix4 half_quarter = {0};
      rotation_x(PI_4, half_quarter);

      matrix4 full_quarter = {0};
      rotation_x(PI_2, full_quarter);

      vec4 hq_actual = {0};
      vec4 hq_expected = point4_init(0, ROOT_2_2, ROOT_2_2);
      matrix4_mulv(half_quarter, p, hq_actual);
      assert(vec4_eq(hq_actual, hq_expected));

      vec4 fq_actual = {0};
      vec4 fq_expected = point4_init(0, 0, 1);
      matrix4_mulv(full_quarter, p, fq_actual);
      assert(vec4_eq(fq_actual, fq_expected));
  }

  TEST {
      // Inverse X rotation
      vec4 p = point4_init(0, 1, 0);

      matrix4 half_quarter = {0};
      rotation_x(PI_4, half_quarter);

      matrix4 TI = {0};
      matrix4_inverse(half_quarter, TI);

      vec4 hq_actual = {0};
      vec4 hq_expected = point4_init(0, ROOT_2_2, -ROOT_2_2);
      matrix4_mulv(TI, p, hq_actual);
      assert(vec4_eq(hq_actual, hq_expected));
  }

  TEST {
      // Rotating a point around the Y axis
      vec4 p = point4_init(0, 0, 1);

      matrix4 half_quarter = {0};
      rotation_y(PI_4, half_quarter);

      matrix4 full_quarter = {0};
      rotation_y(PI_2, full_quarter);

      vec4 hq_actual = {0};
      vec4 hq_expected = point4_init(ROOT_2_2, 0, ROOT_2_2);
      matrix4_mulv(half_quarter, p, hq_actual);
      assert(vec4_eq(hq_actual, hq_expected));

      vec4 fq_actual = {0};
      vec4 fq_expected = point4_init(1, 0, 0);
      matrix4_mulv(full_quarter, p, fq_actual);
      assert(vec4_eq(fq_actual, fq_expected));
  }

  TEST {
      // Rotating a point around the Z axis
      vec4 p = point4_init(0, 1, 0);

      matrix4 half_quarter = {0};
      rotation_z(PI_4, half_quarter);

      matrix4 full_quarter = {0};
      rotation_z(PI_2, full_quarter);

      vec4 hq_actual = {0};
      vec4 hq_expected = point4_init(-ROOT_2_2, ROOT_2_2, 0);
      matrix4_mulv(half_quarter, p, hq_actual);
      assert(vec4_eq(hq_actual, hq_expected));

      vec4 fq_actual = {0};
      vec4 fq_expected = point4_init(-1, 0, 0);
      matrix4_mulv(full_quarter, p, fq_actual);
      assert(vec4_eq(fq_actual, fq_expected));
  }

  TEST {
      // xy shear
      vec4 p = point4_init(2, 3, 4);

      matrix4 T = {0};
      shearing(1, 0, 0, 0, 0, 0, T);

      vec4 actual = {0};
      matrix4_mulv(T, p, actual);

      vec4 expected = point4_init(5, 3, 4);
      assert(vec4_eq(actual, expected));
  }

  TEST {
      // xz shear
      vec4 p = point4_init(2, 3, 4);

      matrix4 T = {0};
      shearing(0, 1, 0, 0, 0, 0, T);

      vec4 actual = {0};
      matrix4_mulv(T, p, actual);

      vec4 expected = point4_init(6, 3, 4);
      assert(vec4_eq(actual, expected));
  }

  TEST {
      // yx shear
      vec4 p = point4_init(2, 3, 4);

      matrix4 T = {0};
      shearing(0, 0, 1, 0, 0, 0, T);

      vec4 actual = {0};
      matrix4_mulv(T, p, actual);

      vec4 expected = point4_init(2, 5, 4);
      assert(vec4_eq(actual, expected));
  }

  TEST {
      // yz shear
      vec4 p = point4_init(2, 3, 4);

      matrix4 T = {0};
      shearing(0, 0, 0, 1, 0, 0, T);

      vec4 actual = {0};
      matrix4_mulv(T, p, actual);

      vec4 expected = point4_init(2, 7, 4);
      assert(vec4_eq(actual, expected));
  }

  TEST {
      // zx shear
      vec4 p = point4_init(2, 3, 4);

      matrix4 T = {0};
      shearing(0, 0, 0, 0, 1, 0, T);

      vec4 actual = {0};
      matrix4_mulv(T, p, actual);

      vec4 expected = point4_init(2, 3, 6);
      assert(vec4_eq(actual, expected));
  }

  TEST {
      // zy shear
      vec4 p = point4_init(2, 3, 4);

      matrix4 T = {0};
      shearing(0, 0, 0, 0, 0, 1, T);

      vec4 actual = {0};
      matrix4_mulv(T, p, actual);

      vec4 expected = point4_init(2, 3, 7);
      assert(vec4_eq(actual, expected));
  }

  TEST {
      // Chaining transforms
      vec4 p = point4_init(1, 0, 1);

      matrix4 A = {0};
      rotation_x(PI_2, A);

      matrix4 B = {0};
      scaling(5, 5, 5, B);

      matrix4 C = {0};
      translation(10, 5, 7, C);

      vec4 p2 = {0};
      matrix4_mulv(A, p, p2);
      assert(vec4_eq(p2, point4(1, -1, 0)));

      vec4 p3 = {0};
      matrix4_mulv(B, p2, p3);
      assert(vec4_eq(p3, point4(5, -5, 0)));

      vec4 p4 = {0};
      matrix4_mulv(C, p3, p4);
      assert(vec4_eq(p4, point4(15, 0, 7)));

      matrix4 T = {0};
      matrix4_mul(B, A, T);
      matrix4_mul(C, T, T);

      vec4 p5 = {0};
      matrix4_mulv(T, p, p5);
      assert(vec4_eq(p5, point4(15, 0, 7)));
  }

  TEST {
      // View transform for default orientation
      vec4 from = point4_init(0, 0, 0);
      vec4 to = point4_init(0, 0, -1);
      vec4 up = vec4_init(0, 1, 0);

      matrix4 out;
      view_transform(from, to, up, out);

      matrix4 expected = {0};
      memcpy(expected, IDENTITY, sizeof(matrix4));

      assert(matrix4_eq(out, expected));
  }

  TEST {
      // View transform looking positive z
      vec4 from = point4_init(0, 0, 0);
      vec4 to = point4_init(0, 0, 1);
      vec4 up = vec4_init(0, 1, 0);

      matrix4 out = {0};
      view_transform(from, to, up, out);

      matrix4 expected = {0};
      scaling(-1, 1, -1, expected);

      assert(matrix4_eq(out, expected));
  }

  TEST {
      // View transform moves the world
      vec4 from = point4_init(0, 0, 8);
      vec4 to = point4_init(0, 0, 0);
      vec4 up = vec4_init(0, 1, 0);

      matrix4 out = {0};
      view_transform(from, to, up, out);

      matrix4 expected = {0};
      translation(0, 0, -8, expected);

      assert(matrix4_eq(out, expected));
  }

  TEST {
      // Arbitrary view transform 
      vec4 from = point4_init(1, 3, 2);
      vec4 to = point4_init(4, -2, 8);
      vec4 up = vec4_init(1, 1, 0);

      matrix4 out = {0};
      view_transform(from, to, up, out);

      matrix4 expected = {
        -0.50709f, 0.50709f,  0.67612f, -2.36643f,
         0.76772f, 0.60609f,  0.12122f, -2.82843f,
        -0.35857f, 0.59761f, -0.71714f,  0.00000f,
         0.00000f, 0.00000f,  0.00000f,  1.00000f,
      };

      assert(matrix4_eq(out, expected));
  }
}

