import math
import unittest

from rtc.matrix import Matrix
from rtc.transform import *
from rtc.tuples import *
from rtc.utils import *


class TestTransform(unittest.TestCase):
    def test_translation(self):
        transform = translation(5, -3, 2)
        p = Point(-3, 4, 5)
        self.assertTrue(tuple4_eq(matrix_mul_tuple(transform, p), Point(2, 1, 7)))

    def test_translation_inverse(self):
        transform = matrix_inverse(translation(5, -3, 2))
        p = Point(-3, 4, 5)
        self.assertTrue(tuple4_eq(matrix_mul_tuple(transform, p), Point(-8, 7, 3)))

    def test_translation_vector(self):
        transform = translation(5, -3, 2)
        v = Vector(-3, 4, 5)
        self.assertTrue(tuple4_eq(matrix_mul_tuple(transform, v), v))

    def test_scaling_point(self):
        transform = scaling(2, 3, 4)
        p = Point(-4, 6, 8)
        self.assertTrue(tuple4_eq(matrix_mul_tuple(transform, p), Point(-8, 18, 32)))

    def test_scaling_vector(self):
        transform = scaling(2, 3, 4)
        p = Vector(-4, 6, 8)
        self.assertTrue(tuple4_eq(matrix_mul_tuple(transform, p), Vector(-8, 18, 32)))

    def test_scaling_inverse(self):
        transform = matrix_inverse(scaling(2, 3, 4))
        p = Vector(-4, 6, 8)
        self.assertTrue(tuple4_eq(matrix_mul_tuple(transform, p), Vector(-2, 2, 2)))

    def test_scaling_reflection(self):
        transform = matrix_inverse(scaling(-1, 1, 1))
        p = Point(2, 3, 4)
        self.assertTrue(tuple4_eq(matrix_mul_tuple(transform, p), Point(-2, 3, 4)))

    def test_rotation_x(self):
        p = Point(0, 1, 0)
        half_quarter = rotation_x(math.pi / 4)
        full_quarter = rotation_x(math.pi / 2)

        self.assertTrue(
            tuple4_eq(
                matrix_mul_tuple(half_quarter, p),
                Point(0, math.sqrt(2) / 2, math.sqrt(2) / 2),
            )
        )
        self.assertTrue(tuple4_eq(matrix_mul_tuple(full_quarter, p), Point(0, 0, 1)))
        self.assertTrue(
            tuple4_eq(
                matrix_mul_tuple(matrix_inverse(half_quarter), p),
                Point(0, math.sqrt(2) / 2, -math.sqrt(2) / 2),
            )
        )

    def test_rotation_y(self):
        p = Point(0, 0, 1)
        half_quarter = rotation_y(math.pi / 4)
        full_quarter = rotation_y(math.pi / 2)

        self.assertTrue(
            tuple4_eq(
                matrix_mul_tuple(half_quarter, p),
                Point(math.sqrt(2) / 2, 0, math.sqrt(2) / 2),
            )
        )
        self.assertTrue(tuple4_eq(matrix_mul_tuple(full_quarter, p), Point(1, 0, 0)))

    def test_rotation_z(self):
        p = Point(0, 1, 0)
        half_quarter = rotation_z(math.pi / 4)
        full_quarter = rotation_z(math.pi / 2)

        self.assertTrue(
            tuple4_eq(
                matrix_mul_tuple(half_quarter, p),
                Point(-math.sqrt(2) / 2, math.sqrt(2) / 2, 0),
            )
        )
        self.assertTrue(tuple4_eq(matrix_mul_tuple(full_quarter, p), Point(-1, 0, 0)))

    def test_shearing(self):
        transform = shearing(1, 0, 0, 0, 0, 0)
        p = Point(2, 3, 4)
        self.assertTrue(tuple4_eq(matrix_mul_tuple(transform, p), Point(5, 3, 4)))

        transform = shearing(0, 1, 0, 0, 0, 0)
        self.assertTrue(tuple4_eq(matrix_mul_tuple(transform, p), Point(6, 3, 4)))

        transform = shearing(0, 0, 1, 0, 0, 0)
        self.assertTrue(tuple4_eq(matrix_mul_tuple(transform, p), Point(2, 5, 4)))

        transform = shearing(0, 0, 0, 1, 0, 0)
        self.assertTrue(tuple4_eq(matrix_mul_tuple(transform, p), Point(2, 7, 4)))

        transform = shearing(0, 0, 0, 0, 1, 0)
        self.assertTrue(tuple4_eq(matrix_mul_tuple(transform, p), Point(2, 3, 6)))

        transform = shearing(0, 0, 0, 0, 0, 1)
        self.assertTrue(tuple4_eq(matrix_mul_tuple(transform, p), Point(2, 3, 7)))

    def test_individual_chaining(self):
        p = Point(1, 0, 1)
        A = rotation_x(math.pi / 2)
        B = scaling(5, 5, 5)
        C = translation(10, 5, 7)

        p2 = matrix_mul_tuple(A, p)
        self.assertTrue(tuple4_eq(p2, Point(1, -1, 0)))

        p3 = matrix_mul_tuple(B, p2)
        self.assertTrue(tuple4_eq(p3, Point(5, -5, 0)))

        p4 = matrix_mul_tuple(C, p3)
        self.assertTrue(tuple4_eq(p4, Point(15, 0, 7)))

    def test_group_chaining(self):
        p = Point(1, 0, 1)
        A = rotation_x(math.pi / 2)
        B = scaling(5, 5, 5)
        C = translation(10, 5, 7)
        T = matrix_mul(C, matrix_mul(B, A))

        self.assertTrue(tuple4_eq(matrix_mul_tuple(T, p), Point(15, 0, 7)))

        # pylint: disable=no-member
        T2 = matrix_mul(
            translation(10, 5, 7), matrix_mul(scaling(5, 5, 5), rotation_x(math.pi / 2))
        )
        self.assertTrue(matrix_eq(T, T2))
        self.assertTrue(tuple4_eq(matrix_mul_tuple(T2, p), Point(15, 0, 7)))

    def test_default_view_transform(self):
        at = Point(0, 0, 0)
        to = Point(0, 0, -1)
        up = Vector(0, 1, 0)
        t = ViewTransform(at, to, up)

        self.assertTrue(matrix_eq(t, IdentityMatrix()))

    def test_view_transform_z(self):
        at = Point(0, 0, 0)
        to = Point(0, 0, 1)
        up = Vector(0, 1, 0)
        t = ViewTransform(at, to, up)

        self.assertTrue(matrix_eq(t, scaling(-1, 1, -1)))

    def test_view_transform_moves_world(self):
        at = Point(0, 0, 8)
        to = Point(0, 0, 0)
        up = Vector(0, 1, 0)
        t = ViewTransform(at, to, up)

        self.assertTrue(matrix_eq(t, translation(0, 0, -8)))

    def test_view_transform_arbitrary(self):
        at = Point(1, 3, 2)
        to = Point(4, -2, 8)
        up = Vector(1, 1, 0)
        t = ViewTransform(at, to, up)

        expected = Matrix(
            -0.50709,
            0.50709,
            0.67612,
            -2.36643,
            0.76772,
            0.60609,
            0.12122,
            -2.82843,
            -0.35857,
            0.59761,
            -0.71714,
            0.00000,
            0.00000,
            0.00000,
            0.00000,
            1.00000,
        )

        self.assertTrue(matrix_eq(t, expected))
