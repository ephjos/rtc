
#include "tests.h"
  
void test_primitives()
{
  TESTS();

  TEST {
      // a v4 with w = 1 is a point
      const v4 a = { 4.3, -4.2, 3.1, 1.0 };
      assert(a[0] == 4.3);
      assert(a[1] == -4.2);
      assert(a[2] == 3.1);
      assert(a[3] == 1.0);
      assert(is_point(a));
      assert(!is_vector(a));
  }

  TEST {
      // a v4 with w = 0 is a point
      const v4 a = { 4.3, -4.2, 3.1, 0.0 };
      assert(a[0] == 4.3);
      assert(a[1] == -4.2);
      assert(a[2] == 3.1);
      assert(a[3] == 0.0);
      assert(!is_point(a));
      assert(is_vector(a));
  }

  TEST {
      // point() creates a v4 with w=1
      assert(v4_eq(point(4, -4, 3), (v4){4, -4, 3, 1}));
  }

  TEST {
      // vector() creates a v4 with w=0
      assert(v4_eq(vector(4, -4, 3), (v4){4, -4, 3, 0}));
  }

  TEST {
      // Adding two v4s
      const v4 a = {3, -2, 5, 1};
      const v4 b = {-2, 3, 1, 0};
      v4 out = {0};
      v4_add(a, b, out);
      assert(v4_eq(out, (v4){1, 1, 6, 1}));
  }

  TEST {
      // Subtracting two points
      const v4 a = point_init(3, 2, 1);
      const v4 b = point_init(5, 6, 7);
      v4 out = {0};
      v4_sub(a, b, out);
      assert(v4_eq(out, vector(-2, -4, -6)));
  }

  TEST {
      // Subtracting a v4 from a point
      const v4 a = point_init(3, 2, 1);
      const v4 b = vector_init(5, 6, 7);
      v4 out = {0};
      v4_sub(a, b, out);
      assert(v4_eq(out, point(-2, -4, -6)));
  }

  TEST {
      // Subtracting two v4s
      const v4 a = vector_init(3, 2, 1);
      const v4 b = vector_init(5, 6, 7);
      v4 out = {0};
      v4_sub(a, b, out);
      assert(v4_eq(out, vector(-2, -4, -6)));
  }

  TEST {
      // Subtracting a v4 from the 0 v4
      const v4 a = vector_init(0, 0, 0);
      const v4 b = vector_init(1, -2, 3);
      v4 out = {0};
      v4_sub(a, b, out);
      assert(v4_eq(out, vector(-1, 2, -3)));
  }

  TEST {
      // Negating a v4
      const v4 a = {1, -2, 3, -4};
      v4 out = {0};
      v4_neg(a, out);
      assert(v4_eq(out, (v4){-1, 2, -3, 4}));
  }

  TEST {
      // Scaling v4
      const v4 a = {1, -2, 3, -4};
      v4 out = {0};
      v4_scale(a, 3.5, out);
      assert(v4_eq(out, (v4){3.5, -7, 10.5, -14}));
  }

  TEST {
      // Scaling const v4 by fraction
      const v4 a = {1, -2, 3, -4};
      v4 out = {0};
      v4_scale(a, 0.5, out);
      assert(v4_eq(out, (v4){0.5, -1, 1.5, -2}));
  }

  TEST {
      // Magnitude of <1, 0, 0>
      const v4 a = vector_init(1, 0, 0);
      f64 res = v4_mag(a);
      assert(req(1, res));
  }

  TEST {
      // Magnitude of <0, 1, 0>
      const v4 a = vector_init(0, 1, 0);
      f64 res = v4_mag(a);
      assert(req(1, res));
  }

  TEST {
      // Magnitude of <0, 0, 1>
      const v4 a = vector_init(0, 0, 1);
      f64 res = v4_mag(a);
      assert(req(1, res));
  }

  TEST {
      // Magnitude of <1, 2, 3>
      const v4 a = vector_init(1, 2, 3);
      f64 res = v4_mag(a);
      assert(req((f64)sqrt(14), res));
  }

  TEST {
      // Magnitude of <-1, -2, -3>
      const v4 a = vector_init(-1, -2, -3);
      f64 res = v4_mag(a);
      assert(req((f64)sqrt(14), res));
  }

  TEST {
      // Normalizaing <4, 0, 0>
      const v4 a = vector_init(4, 0, 0);
      v4 out = {0};
      v4_norm(a, out);
      assert(v4_eq(out, vector(1, 0, 0)));
      assert(req(v4_mag(out), 1));
  }

  TEST {
      // Normalizaing <1, 2, 3>
      const v4 a = vector_init(1, 2, 3);
      v4 out = {0};
      v4_norm(a, out);
      assert(v4_eq(out, vector(0.26726, 0.53452, 0.80178)));
      assert(req(v4_mag(out), 1));
  }

  TEST {
      // The dot product of two v4s
      const v4 a = vector_init(1, 2, 3);
      const v4 b = vector_init(2, 3, 4);
      f64 d = v4_dot(a, b);
      assert(req(d, 20));
  }

  TEST {
      // The cross product of two v4s
      const v4 a = vector_init(1, 2, 3);
      const v4 b = vector_init(2, 3, 4);
      v4 ab = {0};
      v4 ba = {0};
      v4_cross(a, b, ab);
      v4_cross(b, a, ba);
      assert(v4_eq(ab, vector(-1, 2, -1)));
      assert(v4_eq(ba, vector(1, -2, 1)));
  }

  TEST {
      // Adding two colors
      const v3 a = color_init(0.9, 0.6, 0.75);
      const v3 b = color_init(0.7, 0.1, 0.25);
      v3 out = {0};
      v3_add(a, b, out);
      assert(v3_eq(out, color(1.6, 0.7, 1.0)));
  }

  TEST {
      // Subtracting two colors
      const v3 a = color_init(0.9, 0.6, 0.75);
      const v3 b = color_init(0.7, 0.1, 0.25);
      v3 out = {0};
      v3_sub(a, b, out);
      assert(v3_eq(out, color(0.2, 0.5, 0.5)));
  }

  TEST {
      // Scale a color
      const v3 a = color_init(0.2, 0.3, 0.4);
      v3 out = {0};
      v3_scale(a, 2, out);
      assert(v3_eq(out, color(0.4, 0.6, 0.8)));
  }

  TEST {
      // Multiplying two colors
      const v3 a = color_init(1.0, 0.2, 0.4);
      const v3 b = color_init(0.9, 1.0, 0.1);
      v3 out = {0};
      v3_mul(a, b, out);
      assert(v3_eq(out, color(0.9, 0.2, 0.04)));
  }

  TEST {
      // Reflecting a vector approach at 45 deg
      v4 v = vector_init(1, -1, 0);
      v4 n = vector_init(0, 1, 0);

      v4 r = {0};
      v4_reflect(v, n, r);

      assert(v4_eq(r, vector(1, 1, 0)));
  }

  TEST {
      // Reflecting a vector off of a slanted surface
      v4 v = vector_init(0, -1, 0);
      v4 n = vector_init(ROOT_2_2, ROOT_2_2, 0);

      v4 r = {0};
      v4_reflect(v, n, r);

      assert(v4_eq(r, vector(1, 0, 0)));
  }
}
