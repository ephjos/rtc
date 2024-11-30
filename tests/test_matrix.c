
#include "tests.h"

void test_matrix()
{
  TESTS();

  TEST {
      // Constructing a 4x4 matrix
      const matrix4 M = {
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
      const matrix4 A = {
         1, 2, 3, 4,
         5, 6, 7, 8,
         9, 8, 7, 6,
         5, 4, 3, 2,
      };

      const matrix4 B = {
         1, 2, 3, 4,
         5, 6, 7, 8,
         9, 8, 7, 6,
         5, 4, 3, 2,
      };

      assert(matrix4_eq(A, B));
  }

  TEST {
      // Matrix equality with different matrices
      const matrix4 A = {
         1, 2, 3, 4,
         5, 6, 7, 8,
         9, 8, 7, 6,
         5, 4, 3, 2,
      };

      const matrix4 B = {
         9, 8, 7, 6,
         5, 4, 3, 2,
         1, 2, 3, 4,
         5, 6, 7, 8,
      };

      assert(!matrix4_eq(A, B));
  }

  TEST {
      // Matrix multiplication
      const matrix4 A = {
         1, 2, 3, 4,
         5, 6, 7, 8,
         9, 8, 7, 6,
         5, 4, 3, 2,
      };

      const matrix4 B = {
         -2, 1, 2,  3,
          3, 2, 1, -1,
          4, 3, 6,  5,
          1, 2, 7,  8,
      };

      const matrix4 AB = {
         20, 22,  50,  48,
         44, 54, 114, 108,
         40, 58, 110, 102,
         16, 26,  46,  42,
      };

      matrix4 out = {0};
      matrix4_mul(A, B, out);
      assert(matrix4_eq(out, AB));
  }

  TEST {
      // Transposing a matrix
      const matrix4 A = {
        0, 9, 3, 0,
        9, 8, 0, 8,
        1, 8, 5, 3,
        0, 0, 5, 8,
      };

      const matrix4 AT = {
        0, 9, 1, 0,
        9, 8, 8, 0,
        3, 0, 5, 5,
        0, 8, 3, 8
      };

      matrix4 out = {0};
      matrix4_transpose(A, out);
      assert(matrix4_eq(out, AT));
  }

  TEST {
      // Transposing the identity matrix
      matrix4 out = {0};
      matrix4_transpose(IDENTITY, out);
      assert(matrix4_eq(out, IDENTITY));
  }

  TEST {
      // Matrix determinant
      const matrix4 A = {
        -2, -8,  3,  5,
        -3,  1,  7,  3,
         1,  2, -9,  6,
        -6,  7,  7, -9,
      };

      assert(req(matrix4_det(A), -4071));
  }

  TEST {
      // Matrix is invertible
      const matrix4 A = {
        6,  4, 4,  4,
        5,  5, 7,  6,
        4, -9, 3, -7,
        9,  1, 7, -6,
      };

      assert(req(matrix4_det(A), -2120));
  }

  TEST {
      // Matrix is not invertible
      const matrix4 A = {
        -4,  2, -2, -3,
         9,  6,  2,  6, 
         0, -5,  1, -5,
         0,  0,  0,  0,
      };

      assert(req(matrix4_det(A), 0));
  }

  TEST {
      // Inverting a matrix
      const matrix4 A = {
        -5,  2,  6, -8,
         1, -5,  1,  8,
         7,  7, -6, -7,
         1, -3,  7,  4,
      };

      const matrix4 B = {
         0.21805f,  0.45113f,  0.24060f, -0.04511f,
        -0.80827f, -1.45677f, -0.44361f,  0.52068f,
        -0.07895f, -0.22368f, -0.05263f,  0.19737f,
        -0.52256f, -0.81391f, -0.30075f,  0.30639f,
      };

      assert(req(matrix4_det(A), 532));

      matrix4 out = {0};
      matrix4_inverse(A, out);
      assert(matrix4_eq(out, B));
  }

  TEST {
      // Inverting another matrix
      const matrix4 A = {
         8, -5,  9,  2,
         7,  5,  6,  1,
        -6,  0,  9,  6,
        -3,  0, -9, -4,
      };

      const matrix4 B = {
        -0.15385f, -0.15385f, -0.28205f, -0.53846f,
        -0.07692f,  0.12308f,  0.02564f,  0.03077f,
         0.35897f,  0.35897f,  0.43590f,  0.92308f,
        -0.69231f, -0.69231f, -0.76923f, -1.92308f,
      };

      matrix4 out = {0};
      matrix4_inverse(A, out);
      assert(matrix4_eq(out, B));
  }

  TEST {
      // Inverting one more matrix
      const matrix4 A = {
         9,  3,  0,  9,
        -5, -2, -6, -3,
        -4,  9,  6,  4,
        -7,  6,  6,  2,
      };

      const matrix4 B = {
        -0.04074f, -0.07778f,  0.14444f, -0.22222f,
        -0.07778f,  0.03333f,  0.36667f, -0.33333f,
        -0.02901f, -0.14630f, -0.10926f,  0.12963f,
         0.17778f,  0.06667f, -0.26667f,  0.33333f,
      };

      matrix4 out = {0};
      matrix4_inverse(A, out);
      assert(matrix4_eq(out, B));
  }

  TEST {
      // Matrix inversion reverses matrix multiplication
      const matrix4 A = {
         3, -9,  7,  3,
         3, -8,  2, -9,
        -4,  4,  4,  1,
        -6,  5, -1,  1,
      };

      const matrix4 B = {
        8,  2, 2, 2,
        3, -1, 7, 0,
        7,  0, 5, 4,
        6, -2, 0, 5,
      };

      matrix4 C = {0};
      matrix4_mul(A, B, C);

      matrix4 BI = {0};
      matrix4_inverse(B, BI);

      matrix4 CBI = {0};
      matrix4_mul(C, BI, CBI);
      assert(matrix4_eq(CBI, A));
  }
}

