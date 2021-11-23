import unittest

from rtc.matrix import Matrix, IdentityMatrix
from rtc.tuples import Tuple4


class TestMatrix(unittest.TestCase):
    def test_matrix_constructor(self):
        m = Matrix(
            1,
            2,
            3,
            4,
            5.5,
            6.5,
            7.5,
            8.5,
            9,
            10,
            11,
            12,
            13.5,
            14.5,
            15.5,
            16.5,
        )
        self.assertEqual(m.a, 1)
        self.assertEqual(m.d, 4)
        self.assertEqual(m.e, 5.5)
        self.assertEqual(m.g, 7.5)
        self.assertEqual(m.k, 11)
        self.assertEqual(m.m, 13.5)
        self.assertEqual(m.o, 15.5)

    def test_matrix_equality(self):
        A = Matrix(1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2)
        B = Matrix(1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2)
        self.assertEqual(A, B)

    def test_matrix_inequality(self):
        A = Matrix(1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2)
        B = Matrix(2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1)
        self.assertNotEqual(A, B)

    def test_matrix_multiplication(self):
        A = Matrix(1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2)
        B = Matrix(-2, 1, 2, 3, 3, 2, 1, -1, 4, 3, 6, 5, 1, 2, 7, 8)

        expected = Matrix(
            20, 22, 50, 48, 44, 54, 114, 108, 40, 58, 110, 102, 16, 26, 46, 42
        )

        self.assertEqual(A @ B, expected)

    def test_matrix_multiply_tuple(self):
        A = Matrix(1, 2, 3, 4, 2, 4, 4, 2, 8, 6, 4, 1, 0, 0, 0, 1)
        b = Tuple4(1, 2, 3, 1)

        expected = Tuple4(18, 24, 33, 1)

        self.assertEqual(A @ b, expected)

    def test_matrix_multiply_identity(self):
        A = Matrix(0, 1, 2, 4, 1, 2, 4, 8, 2, 4, 8, 16, 4, 8, 16, 32)
        I = IdentityMatrix()
        self.assertEqual(A @ I, A)

    def test_matrix_multiply_identity_tuple(self):
        a = Tuple4(1, 2, 3, 4)
        I = IdentityMatrix()
        self.assertEqual(I @ a, a)

    def test_matrix_transpose(self):
        A = Matrix(0, 9, 3, 0, 9, 8, 0, 8, 1, 8, 5, 3, 0, 0, 5, 8)
        AT = Matrix(0, 9, 1, 0, 9, 8, 8, 0, 3, 0, 5, 5, 0, 8, 3, 8)
        self.assertEqual(A.T, AT)

    def test_matrix_identity_transpose(self):
        A = IdentityMatrix()
        self.assertEqual(A.T, A)

    def test_matrix_4x4_determinant(self):
        A = Matrix(-2, -8, 3, 5, -3, 1, 7, 3, 1, 2, -9, 6, -6, 7, 7, -9)
        self.assertEqual(A.determinant(), -4071)

    def test_matrix_invertible(self):
        A = Matrix(6, 4, 4, 4, 5, 5, 7, 6, 4, -9, 3, -7, 9, 1, 7, -6)
        self.assertEqual(A.determinant(), -2120)
        self.assertTrue(A.is_invertible())

        A = Matrix(-4, 2, -2, -3, 9, 6, 2, 6, 0, -5, 1, -5, 0, 0, 0, 0)
        self.assertEqual(A.determinant(), 0)
        self.assertFalse(A.is_invertible())

    def test_matrix_inverse(self):
        A = Matrix(-5, 2, 6, -8, 1, -5, 1, 8, 7, 7, -6, -7, 1, -3, 7, 4)
        A.inverse()

        expected = Matrix(
            0.21805,
            0.45113,
            0.24060,
            -0.04511,
            -0.80827,
            -1.45677,
            -0.44361,
            0.52068,
            -0.07895,
            -0.22368,
            -0.05263,
            0.19737,
            -0.52256,
            -0.81391,
            -0.30075,
            0.30639,
        )

        self.assertEqual(A.determinant(), 532)

        A = Matrix(8, -5, 9, 2, 7, 5, 6, 1, -6, 0, 9, 6, -3, 0, -9, -4)
        expected = Matrix(
            -0.15385,
            -0.15385,
            -0.28205,
            -0.53846,
            -0.07692,
            0.12308,
            0.02564,
            0.03077,
            0.35897,
            0.35897,
            0.43590,
            0.92308,
            -0.69231,
            -0.69231,
            -0.76923,
            -1.92308,
        )

        self.assertEqual(A.inverse(), expected)

        A = Matrix(9, 3, 0, 9, -5, -2, -6, -3, -4, 9, 6, 4, -7, 6, 6, 2)
        expected = Matrix(
            -0.04074,
            -0.07778,
            0.14444,
            -0.22222,
            -0.07778,
            0.03333,
            0.36667,
            -0.33333,
            -0.02901,
            -0.14630,
            -0.10926,
            0.12963,
            0.17778,
            0.06667,
            -0.26667,
            0.33333,
        )
        self.assertEqual(A.inverse(), expected)

    def test_matrix_multiply_inverse(self):
        A = Matrix(3, -9, 7, 3, 3, -8, 2, -9, -4, 4, 4, 1, -6, 5, 1, 1)
        B = Matrix(8, 2, 2, 2, 3, -1, 7, 0, 7, 0, 5, 4, 6, -2, 0, 5)
        C = A @ B
        self.assertEqual(C @ B.inverse(), A)
