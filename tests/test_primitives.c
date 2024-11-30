
#include "tests.h"
  
void test_primitives()
{
  TESTS();

  TEST {
      // a vec4 with w = 1 is a point4
      const vec4 a = { 4.3, -4.2, 3.1, 1.0 };
      assert(a[0] == 4.3);
      assert(a[1] == -4.2);
      assert(a[2] == 3.1);
      assert(a[3] == 1.0);
      assert(is_point4(a));
      assert(!is_vec4(a));
  }

  TEST {
      // a vec4 with w = 0 is a point4
      const vec4 a = { 4.3, -4.2, 3.1, 0.0 };
      assert(a[0] == 4.3);
      assert(a[1] == -4.2);
      assert(a[2] == 3.1);
      assert(a[3] == 0.0);
      assert(!is_point4(a));
      assert(is_vec4(a));
  }

  TEST {
      // point4() creates a vec4 with w=1
      assert(vec4_eq(point4(4, -4, 3), (vec4){4, -4, 3, 1}));
  }

  TEST {
      // vec4() creates a vec4 with w=0
      assert(vec4_eq(vec4(4, -4, 3), (vec4){4, -4, 3, 0}));
  }

  TEST {
      // Adding two vec4s
      const vec4 a = {3, -2, 5, 1};
      const vec4 b = {-2, 3, 1, 0};
      vec4 out = {0};
      vec4_add(a, b, out);
      assert(vec4_eq(out, (vec4){1, 1, 6, 1}));
  }

  TEST {
      // Subtracting two point4s
      const vec4 a = point4_init(3, 2, 1);
      const vec4 b = point4_init(5, 6, 7);
      vec4 out = {0};
      vec4_sub(a, b, out);
      assert(vec4_eq(out, vec4(-2, -4, -6)));
  }

  TEST {
      // Subtracting a vec4 from a point4
      const vec4 a = point4_init(3, 2, 1);
      const vec4 b = vec4_init(5, 6, 7);
      vec4 out = {0};
      vec4_sub(a, b, out);
      assert(vec4_eq(out, point4(-2, -4, -6)));
  }

  TEST {
      // Subtracting two vec4s
      const vec4 a = vec4_init(3, 2, 1);
      const vec4 b = vec4_init(5, 6, 7);
      vec4 out = {0};
      vec4_sub(a, b, out);
      assert(vec4_eq(out, vec4(-2, -4, -6)));
  }

  TEST {
      // Subtracting a vec4 from the 0 vec4
      const vec4 a = vec4_init(0, 0, 0);
      const vec4 b = vec4_init(1, -2, 3);
      vec4 out = {0};
      vec4_sub(a, b, out);
      assert(vec4_eq(out, vec4(-1, 2, -3)));
  }

  TEST {
      // Negating a vec4
      const vec4 a = {1, -2, 3, -4};
      vec4 out = {0};
      vec4_neg(a, out);
      assert(vec4_eq(out, (vec4){-1, 2, -3, 4}));
  }

  TEST {
      // Scaling vec4
      const vec4 a = {1, -2, 3, -4};
      vec4 out = {0};
      vec4_scale(a, 3.5, out);
      assert(vec4_eq(out, (vec4){3.5, -7, 10.5, -14}));
  }

  TEST {
      // Scaling const vec4 by fraction
      const vec4 a = {1, -2, 3, -4};
      vec4 out = {0};
      vec4_scale(a, 0.5, out);
      assert(vec4_eq(out, (vec4){0.5, -1, 1.5, -2}));
  }

  TEST {
      // Magnitude of <1, 0, 0>
      const vec4 a = vec4_init(1, 0, 0);
      f64 res = vec4_mag(a);
      assert(req(1, res));
  }

  TEST {
      // Magnitude of <0, 1, 0>
      const vec4 a = vec4_init(0, 1, 0);
      f64 res = vec4_mag(a);
      assert(req(1, res));
  }

  TEST {
      // Magnitude of <0, 0, 1>
      const vec4 a = vec4_init(0, 0, 1);
      f64 res = vec4_mag(a);
      assert(req(1, res));
  }

  TEST {
      // Magnitude of <1, 2, 3>
      const vec4 a = vec4_init(1, 2, 3);
      f64 res = vec4_mag(a);
      assert(req((f64)sqrt(14), res));
  }

  TEST {
      // Magnitude of <-1, -2, -3>
      const vec4 a = vec4_init(-1, -2, -3);
      f64 res = vec4_mag(a);
      assert(req((f64)sqrt(14), res));
  }

  TEST {
      // Normalizaing <4, 0, 0>
      const vec4 a = vec4_init(4, 0, 0);
      vec4 out = {0};
      vec4_norm(a, out);
      assert(vec4_eq(out, vec4(1, 0, 0)));
      assert(req(vec4_mag(out), 1));
  }

  TEST {
      // Normalizaing <1, 2, 3>
      const vec4 a = vec4_init(1, 2, 3);
      vec4 out = {0};
      vec4_norm(a, out);
      assert(vec4_eq(out, vec4(0.26726f, 0.53452f, 0.80178f)));
      assert(req(vec4_mag(out), 1));
  }

  TEST {
      // The dot product of two vec4s
      const vec4 a = vec4_init(1, 2, 3);
      const vec4 b = vec4_init(2, 3, 4);
      f64 d = vec4_dot(a, b);
      assert(req(d, 20));
  }

  TEST {
      // The cross product of two vec4s
      const vec4 a = vec4_init(1, 2, 3);
      const vec4 b = vec4_init(2, 3, 4);
      vec4 ab = {0};
      vec4 ba = {0};
      vec4_cross(a, b, ab);
      vec4_cross(b, a, ba);
      assert(vec4_eq(ab, vec4(-1, 2, -1)));
      assert(vec4_eq(ba, vec4(1, -2, 1)));
  }

  TEST {
      // Adding two colors
      const vec4 a = color_init(0.9, 0.6, 0.75f);
      const vec4 b = color_init(0.7, 0.1, 0.25f);
      vec4 out = {0};
      vec4_add(a, b, out);
      assert(vec4_eq(out, color(1.6, 0.7, 1.0)));
  }

  TEST {
      // Subtracting two colors
      const vec4 a = color_init(0.9, 0.6, 0.75f);
      const vec4 b = color_init(0.7, 0.1, 0.25f);
      vec4 out = {0};
      vec4_sub(a, b, out);
      assert(vec4_eq(out, color(0.2, 0.5, 0.5)));
  }

  TEST {
      // Scale a color
      const vec4 a = color_init(0.2, 0.3, 0.4);
      vec4 out = {0};
      vec4_scale(a, 2, out);
      assert(vec4_eq(out, color(0.4, 0.6, 0.8)));
  }

  TEST {
      // Multiplying two colors
      const vec4 a = color_init(1.0, 0.2, 0.4);
      const vec4 b = color_init(0.9, 1.0, 0.1);
      vec4 out = {0};
      vec4_mul(a, b, out);
      assert(vec4_eq(out, color(0.9, 0.2, 0.04f)));
  }

  TEST {
      // Reflecting a vector approach at 45 deg
      vec4 v = vec4_init(1, -1, 0);
      vec4 n = vec4_init(0, 1, 0);

      vec4 r = {0};
      vec4_reflect(v, n, r);

      assert(vec4_eq(r, vec4(1, 1, 0)));
  }

  TEST {
      // Reflecting a vector off of a slanted surface
      vec4 v = vec4_init(0, -1, 0);
      vec4 n = vec4_init(ROOT_2_2, ROOT_2_2, 0);

      vec4 r = {0};
      vec4_reflect(v, n, r);

      assert(vec4_eq(r, vec4(1, 0, 0)));
  }
}
