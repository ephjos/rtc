
#include "tests.h"

void test_matrix(void)
{
  TESTS();

  TEST {
      // Constructing a 4x4 matrix
      const m4 M = {
         1,     2,     3,     4    ,
         5.5,  6.5,  7.5,  8.5 ,
         9,     10,    11,    12   ,
         13.5, 14.5, 15.5, 16.5, 
      };

      assert(req(M[0 _ 0], 1));
      assert(req(M[0 _ 3], 4));
      assert(req(M[1 _ 0], 5.5));
      assert(req(M[1 _ 2], 7.5));
      assert(req(M[2 _ 2], 11));
      assert(req(M[3 _ 0], 13.5));
      assert(req(M[3 _ 2], 15.5));
  }

  TEST {
      // Matrix equality with same matrices
      const m4 A = {
         1, 2, 3, 4,
         5, 6, 7, 8,
         9, 8, 7, 6,
         5, 4, 3, 2,
      };

      const m4 B = {
         1, 2, 3, 4,
         5, 6, 7, 8,
         9, 8, 7, 6,
         5, 4, 3, 2,
      };

      assert(m4_eq(A, B));
  }

  TEST {
      // Matrix equality with different matrices
      const m4 A = {
         1, 2, 3, 4,
         5, 6, 7, 8,
         9, 8, 7, 6,
         5, 4, 3, 2,
      };

      const m4 B = {
         9, 8, 7, 6,
         5, 4, 3, 2,
         1, 2, 3, 4,
         5, 6, 7, 8,
      };

      assert(!m4_eq(A, B));
  }

  TEST {
      // Matrix multiplication
      const m4 A = {
         1, 2, 3, 4,
         5, 6, 7, 8,
         9, 8, 7, 6,
         5, 4, 3, 2,
      };

      const m4 B = {
         -2, 1, 2,  3,
          3, 2, 1, -1,
          4, 3, 6,  5,
          1, 2, 7,  8,
      };

      const m4 AB = {
         20, 22,  50,  48,
         44, 54, 114, 108,
         40, 58, 110, 102,
         16, 26,  46,  42,
      };

      m4 out = {0};
      m4_mul(A, B, out);
      assert(m4_eq(out, AB));
  }

  TEST {
      // Transposing a matrix
      const m4 A = {
        0, 9, 3, 0,
        9, 8, 0, 8,
        1, 8, 5, 3,
        0, 0, 5, 8,
      };

      const m4 AT = {
        0, 9, 1, 0,
        9, 8, 8, 0,
        3, 0, 5, 5,
        0, 8, 3, 8
      };

      m4 out = {0};
      m4_transpose(A, out);
      assert(m4_eq(out, AT));
  }

  TEST {
      // Transposing the identity matrix
      m4 out = {0};
      m4_transpose(IDENTITY, out);
      assert(m4_eq(out, IDENTITY));
  }

  TEST {
      // Matrix determinant
      const m4 A = {
        -2, -8,  3,  5,
        -3,  1,  7,  3,
         1,  2, -9,  6,
        -6,  7,  7, -9,
      };

      assert(req(m4_det(A), -4071));
  }

  TEST {
      // Matrix is invertible
      const m4 A = {
        6,  4, 4,  4,
        5,  5, 7,  6,
        4, -9, 3, -7,
        9,  1, 7, -6,
      };

      assert(req(m4_det(A), -2120));
  }

  TEST {
      // Matrix is not invertible
      const m4 A = {
        -4,  2, -2, -3,
         9,  6,  2,  6, 
         0, -5,  1, -5,
         0,  0,  0,  0,
      };

      assert(req(m4_det(A), 0));
  }

  TEST {
      // Inverting a matrix
      const m4 A = {
        -5,  2,  6, -8,
         1, -5,  1,  8,
         7,  7, -6, -7,
         1, -3,  7,  4,
      };

      const m4 B = {
         0.21805,  0.45113,  0.24060, -0.04511,
        -0.80827, -1.45677, -0.44361,  0.52068,
        -0.07895, -0.22368, -0.05263,  0.19737,
        -0.52256, -0.81391, -0.30075,  0.30639,
      };

      assert(req(m4_det(A), 532));

      m4 out = {0};
      m4_inverse(A, out);
      assert(m4_eq(out, B));
  }

  TEST {
      // Inverting another matrix
      const m4 A = {
         8, -5,  9,  2,
         7,  5,  6,  1,
        -6,  0,  9,  6,
        -3,  0, -9, -4,
      };

      const m4 B = {
        -0.15385, -0.15385, -0.28205, -0.53846,
        -0.07692,  0.12308,  0.02564,  0.03077,
         0.35897,  0.35897,  0.43590,  0.92308,
        -0.69231, -0.69231, -0.76923, -1.92308,
      };

      m4 out = {0};
      m4_inverse(A, out);
      assert(m4_eq(out, B));
  }

  TEST {
      // Inverting one more matrix
      const m4 A = {
         9,  3,  0,  9,
        -5, -2, -6, -3,
        -4,  9,  6,  4,
        -7,  6,  6,  2,
      };

      const m4 B = {
        -0.04074, -0.07778,  0.14444, -0.22222,
        -0.07778,  0.03333,  0.36667, -0.33333,
        -0.02901, -0.14630, -0.10926,  0.12963,
         0.17778,  0.06667, -0.26667,  0.33333,
      };

      m4 out = {0};
      m4_inverse(A, out);
      assert(m4_eq(out, B));
  }

  TEST {
      // Matrix inversion reverses matrix multiplication
      const m4 A = {
         3, -9,  7,  3,
         3, -8,  2, -9,
        -4,  4,  4,  1,
        -6,  5, -1,  1,
      };

      const m4 B = {
        8,  2, 2, 2,
        3, -1, 7, 0,
        7,  0, 5, 4,
        6, -2, 0, 5,
      };

      m4 C = {0};
      m4_mul(A, B, C);

      m4 BI = {0};
      m4_inverse(B, BI);

      m4 CBI = {0};
      m4_mul(C, BI, CBI);
      assert(m4_eq(CBI, A));
  }
}

