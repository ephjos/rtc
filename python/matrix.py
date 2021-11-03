#!/usr/bin/env python
import copy
import math
import unittest

from tuples import Tuple
from utils import req, clamp

class Matrix:
    def __init__(self, rows):
        self.rows = rows

    def __getitem__(self, key):
        return self.rows[key]

    def __str__(self):
        return str(self.rows)

    def __repr__(self):
        return str(self)

    @property
    def shape(self):
        if len(self.rows) == 0:
            return [0, 0]
        return [len(self.rows), len(self.rows[0])]

    def __eq__(self, o):
        shape = self.shape
        if shape != o.shape:
            return False

        for i in range(shape[0]):
            for j in range(shape[1]):
                if not req(self[i][j], o[i][j]):
                    return False

        return True

    def __matmul_matrix__(self, o):
        [p, q] = self.shape
        [r, s] = o.shape

        if q != r:
            raise Exception("Shape mismatch: " + str([p,q]) + " " + str([r,s]))

        M = Matrix([[0 for j in range(s)] for i in range(r)])

        for i in range(p):
            for j in range(s):
                M[i][j] = 0
                for k in range(r):
                    M[i][j] += self[i][k] * o[k][j]

        return M

    def __matmul_tuple__(self, o):
        result = self.__matmul_matrix__(Matrix([[o.x],[o.y],[o.z],[o.w]]))
        return Tuple(result[0][0], result[1][0], result[2][0], result[3][0])

    def __matmul__(self, o):
        if isinstance(o, Matrix):
            return self.__matmul_matrix__(o)
        else:
            return self.__matmul_tuple__(o)

    @staticmethod
    def identity(d=4):
        return Matrix([[1 if i == j else 0 for j in range(d)] for i in range(d)])

    @property
    def T(self):
        return Matrix(list(map(list, zip(*self.rows))))

    def determinant(self):
        [p, q] = self.shape
        if p != q:
            raise Exception("Cannot get determinant of non-square matrix")

        if p == 2:
            a = self.rows[0][0]
            b = self.rows[0][1]
            c = self.rows[1][0]
            d = self.rows[1][1]
            return (a*d) - (b*c)

        result = 0
        for i in range(p):
            result += self[0][i] * self.cofactor(0,i)
        return result

    def submatrix(self, y, x):
        [p, q] = self.shape
        if p != q:
            raise Exception("Cannot get determinant of non-square matrix")

        rows = copy.deepcopy(self.rows)
        del rows[y]
        for row in rows:
            del row[x]

        return Matrix(rows)

    def minor(self, y, x):
        return self.submatrix(y,x).determinant()

    def cofactor(self, y, x):
        m = self.minor(y,x)
        if (x+y) % 2 != 0:
            return -1 * m
        return m

    def is_invertible(self):
        return self.determinant() != 0

    def inverse(self):
        [p, q] = self.shape
        if p != q:
            raise Exception("Cannot get determinant of non-square matrix")

        det = self.determinant()
        cofactors = Matrix([[self.cofactor(i,j)/det for j in range(p)] for i in range(p)])
        return cofactors.T



class TestMatrix(unittest.TestCase):
    def test_matrix_constructor(self):
        m = Matrix([[1,    2,    3,    4],
                    [5.5,  6.5,  7.5,  8.5],
                    [9,    10,   11,   12],
                    [13.5, 14.5, 15.5, 16.5]])
        self.assertEqual(m[0][0], 1)
        self.assertEqual(m[0][3], 4)
        self.assertEqual(m[1][0], 5.5)
        self.assertEqual(m[1][2], 7.5)
        self.assertEqual(m[2][2], 11)
        self.assertEqual(m[3][0], 13.5)
        self.assertEqual(m[3][2], 15.5)

    def test_matrix_2x2_constructor(self):
        m = Matrix([[-3,  5],
                    [1,  -2]])
        self.assertEqual(m[0][0], -3)
        self.assertEqual(m[0][1], 5)
        self.assertEqual(m[1][0], 1)
        self.assertEqual(m[1][1], -2)

    def test_matrix_3x3_constructor(self):
        m = Matrix([[-3,  5,  0],
                    [1,  -2,  -7],
                    [0,   1,  1]])
        self.assertEqual(m[0][0], -3)
        self.assertEqual(m[1][1], -2)
        self.assertEqual(m[2][2], 1)

    def test_matrix_equality(self):
        A = Matrix([[1,    2,    3,    4],
                    [5,    6,    7,    8],
                    [9,    8,    7,    6],
                    [5,    4,    3,    2]])
        B = Matrix([[1,    2,    3,    4],
                    [5,    6,    7,    8],
                    [9,    8,    7,    6],
                    [5,    4,    3,    2]])
        self.assertEqual(A, B)

    def test_matrix_inequality(self):
        A = Matrix([[1,    2,    3,    4],
                    [5,    6,    7,    8],
                    [9,    8,    7,    6],
                    [5,    4,    3,    2]])
        B = Matrix([[2,    3,    4,    5],
                    [6,    7,    8,    9],
                    [8,    7,    6,    5],
                    [4,    3,    2,    1]])
        self.assertNotEqual(A, B)

    def test_matrix_multiplication(self):
        A = Matrix([[1,2,3,4],
                    [5,6,7,8],
                    [9,8,7,6],
                    [5,4,3,2]])
        B = Matrix([[-2,1,2,3],
                    [3,2,1,-1],
                    [4,3,6,5],
                    [1,2,7,8]])

        expected = Matrix([[20,22,50,48],
                           [44,54,114,108],
                           [40,58,110,102],
                           [16,26,46,42]])

        self.assertEqual(A @ B, expected)

    def test_matrix_multiply_tuple(self):
        A = Matrix([[1,2,3,4],
                    [2,4,4,2],
                    [8,6,4,1],
                    [0,0,0,1]])
        b = Tuple(1,2,3,1)

        expected = Tuple(18,24,33,1)

        self.assertEqual(A @ b, expected)

    def test_matrix_multiply_identity(self):
        A = Matrix([[0,1,2,4],
                    [1,2,4,8],
                    [2,4,8,16],
                    [4,8,16,32]])
        I = Matrix.identity(4)
        self.assertEqual(A @ I, A)

    def test_matrix_multiply_identity_tuple(self):
        a = Tuple(1,2,3,4)
        I = Matrix.identity(4)
        self.assertEqual(I @ a, a)

    def test_matrix_transpose(self):
        A = Matrix([[0,9,3,0],
                    [9,8,0,8],
                    [1,8,5,3],
                    [0,0,5,8]])
        AT = Matrix([[0,9,1,0],
                     [9,8,8,0],
                     [3,0,5,5],
                     [0,8,3,8]])
        self.assertEqual(A.T, AT)

    def test_matrix_identity_transpose(self):
        A = Matrix.identity(4)
        self.assertEqual(A.T, A)

    def test_matrix_submatrix(self):
        A = Matrix([[1,5,0],
                    [-3,2,7],
                    [0,6,-3]])
        expected = Matrix([[-3,2],
                           [0,6]])
        self.assertEqual(A.submatrix(0,2), expected)

        A = Matrix([[-6,1,1,6],
                    [-8,5,8,6],
                    [-1,0,8,2],
                    [-7,1,-1,1]])
        expected = Matrix([[-6,1,6],
                           [-8,8,6],
                           [-7,-1,1]])
        self.assertEqual(A.submatrix(2,1), expected)

    def test_matrix_3x3_minor(self):
        A = Matrix([[3,5,0],
                    [2,-1,-7],
                    [6,-1,5]])
        B = A.submatrix(1,0)

        self.assertEqual(B.determinant(), 25)
        self.assertEqual(A.minor(1,0), 25)

    def test_matrix_3x3_cofactor(self):
        A = Matrix([[3,5,0],
                    [2,-1,-7],
                    [6,-1,5]])
        self.assertEqual(A.minor(0,0), -12)
        self.assertEqual(A.cofactor(0,0), -12)
        self.assertEqual(A.minor(1,0), 25)
        self.assertEqual(A.cofactor(1,0), -25)

    def test_matrix_3x3_determinant(self):
        A = Matrix([[1,2,6],
                    [-5,8,-4],
                    [2,6,4]])
        self.assertEqual(A.cofactor(0,0), 56)
        self.assertEqual(A.cofactor(0,1), 12)
        self.assertEqual(A.cofactor(0,2), -46)
        self.assertEqual(A.determinant(), -196)

    def test_matrix_4x4_determinant(self):
        A = Matrix([[-2,-8,3,5],
                    [-3,1,7,3],
                    [1,2,-9,6],
                    [-6,7,7,-9]])
        self.assertEqual(A.cofactor(0,0), 690)
        self.assertEqual(A.cofactor(0,1), 447)
        self.assertEqual(A.cofactor(0,2), 210)
        self.assertEqual(A.cofactor(0,3), 51)
        self.assertEqual(A.determinant(), -4071)

    def test_matrix_invertible(self):
        A = Matrix([[6,4,4,4],
                    [5,5,7,6],
                    [4,-9,3,-7],
                    [9,1,7,-6]])
        self.assertEqual(A.determinant(), -2120)
        self.assertTrue(A.is_invertible())

        A = Matrix([[-4,2,-2,-3],
                    [9,6,2,6],
                    [0,-5,1,-5],
                    [0,0,0,0]])
        self.assertEqual(A.determinant(), 0)
        self.assertFalse(A.is_invertible())

    def test_matrix_inverse(self):
        A = Matrix([[-5,2,6,-8],
                    [1,-5,1,8],
                    [7,7,-6,-7],
                    [1,-3,7,4]])
        B = A.inverse()

        expected = Matrix([[0.21805, 0.45113, 0.24060, -0.04511],
                           [-0.80827, -1.45677, -0.44361, 0.52068],
                           [-0.07895, -0.22368, -0.05263, 0.19737],
                           [-0.52256, -0.81391, -0.30075, 0.30639]])

        self.assertEqual(A.determinant(), 532)
        self.assertEqual(A.cofactor(2,3), -160)
        self.assertEqual(B[3][2], -160/532)
        self.assertEqual(A.cofactor(3,2), 105)
        self.assertEqual(B[2][3], 105/532)

        A = Matrix([[8, -5,  9,  2],
                    [7,  5,  6,  1],
                    [-6,  0,  9,  6],
                    [-3,  0, -9, -4]])
        expected = Matrix([[-0.15385, -0.15385, -0.28205, -0.53846],
                           [-0.07692,  0.12308,  0.02564,  0.03077],
                           [0.35897,  0.35897,  0.43590,  0.92308],
                           [-0.69231, -0.69231, -0.76923, -1.92308]])

        self.assertEqual(A.inverse(), expected)

        A = Matrix([[9,  3,  0,  9],
                    [-5, -2, -6, -3],
                    [-4,  9,  6,  4],
                    [-7,  6,  6,  2]])
        expected = Matrix([[-0.04074, -0.07778,  0.14444, -0.22222],
                           [-0.07778,  0.03333,  0.36667, -0.33333],
                           [-0.02901, -0.14630, -0.10926,  0.12963],
                           [0.17778,  0.06667, -0.26667,  0.33333]])
        self.assertEqual(A.inverse(), expected)

    def test_matrix_multiply_inverse(self):
        A = Matrix([[3,-9,7,3],
                    [3,-8,2,-9],
                    [-4,4,4,1],
                    [-6,5,1,1]])
        B = Matrix([[8,2,2,2],
                    [3,-1,7,0],
                    [7,0,5,4],
                    [6,-2,0,5]])
        C = A @ B
        self.assertEqual(C @ B.inverse(), A)
