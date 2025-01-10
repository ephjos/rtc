
#include "tests.h"

void test_transform()
{
  TESTS();

  TEST {
      // Multiplying by a translation matrix
      v4 p = point_init(-3, 4, 5);

      m4 T = {0};
      translation(5, -3, 2, T);

      v4 actual = {0};
      m4_mulv(T, p, actual);

      v4 expected = point_init(2, 1, 7);

      assert(v4_eq(actual, expected));
  }

  TEST {
      // Multiplying by inverse translation matrix
      v4 p = point_init(-3, 4, 5);

      m4 T = {0};
      translation(5, -3, 2, T);

      m4 TI = {0};
      m4_inverse(T, TI);

      v4 actual = {0};
      m4_mulv(TI, p, actual);

      v4 expected = point_init(-8, 7, 3);

      assert(v4_eq(actual, expected));
  }

  TEST {
      // Can't translate vectors
      v4 v = vector_init(-3, 4, 5);

      m4 T = {0};
      translation(5, -3, 2, T);

      v4 actual = {0};
      m4_mulv(T, v, actual);

      assert(v4_eq(actual, v));
  }

  TEST {
      // Scaling a point
      v4 p = point_init(-4, 6, 8);

      m4 T = {0};
      scaling(2, 3, 4, T);

      v4 actual = {0};
      m4_mulv(T, p, actual);

      v4 expected = point_init(-8, 18, 32);

      assert(v4_eq(actual, expected));
  }

  TEST {
      // Scaling a vector
      v4 v = vector_init(-4, 6, 8);

      m4 T = {0};
      scaling(2, 3, 4, T);

      v4 actual = {0};
      m4_mulv(T, v, actual);

      v4 expected = vector_init(-8, 18, 32);

      assert(v4_eq(actual, expected));
  }

  TEST {
      // Inverse scaling a vector
      v4 v = vector_init(-4, 6, 8);

      m4 T = {0};
      scaling(2, 3, 4, T);

      m4 TI = {0};
      m4_inverse(T, TI);

      v4 actual = {0};
      m4_mulv(TI, v, actual);

      v4 expected = vector_init(-2, 2, 2);

      assert(v4_eq(actual, expected));
  }

  TEST {
      // Reflection is scaling by a negative value
      v4 v = vector_init(2, 3, 4);

      m4 T = {0};
      scaling(-1, 1, 1, T);

      v4 actual = {0};
      m4_mulv(T, v, actual);

      v4 expected = vector_init(-2, 3, 4);

      assert(v4_eq(actual, expected));
  }

  TEST {
      // Rotating a point around the X axis
      v4 p = point_init(0, 1, 0);

      m4 half_quarter = {0};
      rotation_x(PI_4, half_quarter);

      m4 full_quarter = {0};
      rotation_x(PI_2, full_quarter);

      v4 hq_actual = {0};
      v4 hq_expected = point_init(0, ROOT_2_2, ROOT_2_2);
      m4_mulv(half_quarter, p, hq_actual);
      assert(v4_eq(hq_actual, hq_expected));

      v4 fq_actual = {0};
      v4 fq_expected = point_init(0, 0, 1);
      m4_mulv(full_quarter, p, fq_actual);
      assert(v4_eq(fq_actual, fq_expected));
  }

  TEST {
      // Inverse X rotation
      v4 p = point_init(0, 1, 0);

      m4 half_quarter = {0};
      rotation_x(PI_4, half_quarter);

      m4 TI = {0};
      m4_inverse(half_quarter, TI);

      v4 hq_actual = {0};
      v4 hq_expected = point_init(0, ROOT_2_2, -ROOT_2_2);
      m4_mulv(TI, p, hq_actual);
      assert(v4_eq(hq_actual, hq_expected));
  }

  TEST {
      // Rotating a point around the Y axis
      v4 p = point_init(0, 0, 1);

      m4 half_quarter = {0};
      rotation_y(PI_4, half_quarter);

      m4 full_quarter = {0};
      rotation_y(PI_2, full_quarter);

      v4 hq_actual = {0};
      v4 hq_expected = point_init(ROOT_2_2, 0, ROOT_2_2);
      m4_mulv(half_quarter, p, hq_actual);
      assert(v4_eq(hq_actual, hq_expected));

      v4 fq_actual = {0};
      v4 fq_expected = point_init(1, 0, 0);
      m4_mulv(full_quarter, p, fq_actual);
      assert(v4_eq(fq_actual, fq_expected));
  }

  TEST {
      // Rotating a point around the Z axis
      v4 p = point_init(0, 1, 0);

      m4 half_quarter = {0};
      rotation_z(PI_4, half_quarter);

      m4 full_quarter = {0};
      rotation_z(PI_2, full_quarter);

      v4 hq_actual = {0};
      v4 hq_expected = point_init(-ROOT_2_2, ROOT_2_2, 0);
      m4_mulv(half_quarter, p, hq_actual);
      assert(v4_eq(hq_actual, hq_expected));

      v4 fq_actual = {0};
      v4 fq_expected = point_init(-1, 0, 0);
      m4_mulv(full_quarter, p, fq_actual);
      assert(v4_eq(fq_actual, fq_expected));
  }

  TEST {
      // xy shear
      v4 p = point_init(2, 3, 4);

      m4 T = {0};
      shearing(1, 0, 0, 0, 0, 0, T);

      v4 actual = {0};
      m4_mulv(T, p, actual);

      v4 expected = point_init(5, 3, 4);
      assert(v4_eq(actual, expected));
  }

  TEST {
      // xz shear
      v4 p = point_init(2, 3, 4);

      m4 T = {0};
      shearing(0, 1, 0, 0, 0, 0, T);

      v4 actual = {0};
      m4_mulv(T, p, actual);

      v4 expected = point_init(6, 3, 4);
      assert(v4_eq(actual, expected));
  }

  TEST {
      // yx shear
      v4 p = point_init(2, 3, 4);

      m4 T = {0};
      shearing(0, 0, 1, 0, 0, 0, T);

      v4 actual = {0};
      m4_mulv(T, p, actual);

      v4 expected = point_init(2, 5, 4);
      assert(v4_eq(actual, expected));
  }

  TEST {
      // yz shear
      v4 p = point_init(2, 3, 4);

      m4 T = {0};
      shearing(0, 0, 0, 1, 0, 0, T);

      v4 actual = {0};
      m4_mulv(T, p, actual);

      v4 expected = point_init(2, 7, 4);
      assert(v4_eq(actual, expected));
  }

  TEST {
      // zx shear
      v4 p = point_init(2, 3, 4);

      m4 T = {0};
      shearing(0, 0, 0, 0, 1, 0, T);

      v4 actual = {0};
      m4_mulv(T, p, actual);

      v4 expected = point_init(2, 3, 6);
      assert(v4_eq(actual, expected));
  }

  TEST {
      // zy shear
      v4 p = point_init(2, 3, 4);

      m4 T = {0};
      shearing(0, 0, 0, 0, 0, 1, T);

      v4 actual = {0};
      m4_mulv(T, p, actual);

      v4 expected = point_init(2, 3, 7);
      assert(v4_eq(actual, expected));
  }

  TEST {
      // Chaining transforms
      v4 p = point_init(1, 0, 1);

      m4 A = {0};
      rotation_x(PI_2, A);

      m4 B = {0};
      scaling(5, 5, 5, B);

      m4 C = {0};
      translation(10, 5, 7, C);

      v4 p2 = {0};
      m4_mulv(A, p, p2);
      assert(v4_eq(p2, point(1, -1, 0)));

      v4 p3 = {0};
      m4_mulv(B, p2, p3);
      assert(v4_eq(p3, point(5, -5, 0)));

      v4 p4 = {0};
      m4_mulv(C, p3, p4);
      assert(v4_eq(p4, point(15, 0, 7)));

      m4 T = {0};
      m4_mul(B, A, T);
      m4_mul(C, T, T);

      v4 p5 = {0};
      m4_mulv(T, p, p5);
      assert(v4_eq(p5, point(15, 0, 7)));
  }

  TEST {
      // View transform for default orientation
      v4 from = point_init(0, 0, 0);
      v4 to = point_init(0, 0, -1);
      v4 up = vector_init(0, 1, 0);

      m4 out;
      view_transform(from, to, up, out);

      m4 expected = {0};
      memcpy(expected, IDENTITY, sizeof(m4));

      assert(m4_eq(out, expected));
  }

  TEST {
      // View transform looking positive z
      v4 from = point_init(0, 0, 0);
      v4 to = point_init(0, 0, 1);
      v4 up = vector_init(0, 1, 0);

      m4 out = {0};
      view_transform(from, to, up, out);

      m4 expected = {0};
      scaling(-1, 1, -1, expected);

      assert(m4_eq(out, expected));
  }

  TEST {
      // View transform moves the world
      v4 from = point_init(0, 0, 8);
      v4 to = point_init(0, 0, 0);
      v4 up = vector_init(0, 1, 0);

      m4 out = {0};
      view_transform(from, to, up, out);

      m4 expected = {0};
      translation(0, 0, -8, expected);

      assert(m4_eq(out, expected));
  }

  TEST {
      // Arbitrary view transform 
      v4 from = point_init(1, 3, 2);
      v4 to = point_init(4, -2, 8);
      v4 up = vector_init(1, 1, 0);

      m4 out = {0};
      view_transform(from, to, up, out);

      m4 expected = {
        -0.50709, 0.50709,  0.67612, -2.36643,
         0.76772, 0.60609,  0.12122, -2.82843,
        -0.35857, 0.59761, -0.71714,  0.00000,
         0.00000, 0.00000,  0.00000,  1.00000,
      };

      assert(m4_eq(out, expected));
  }
}

