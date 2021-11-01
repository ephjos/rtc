#!/usr/bin/env python
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

    def shape(self):
        if len(self.rows) == 0:
            return [0, 0]
        return [len(self.rows), len(self.rows[0])]

    def __eq__(self, o):
        shape = self.shape()
        if shape != o.shape():
            return False

        for i in range(shape[0]):
            for j in range(shape[1]):
                if not req(self[i][j], o[i][j]):
                    return False

        return True

    def __matmul_matrix__(self, o):
        [p, q] = self.shape()
        [r, s] = o.shape()

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


