import math
import unittest

from rtc.matrix import Matrix
from rtc.transform import Transform, ViewTransform
from rtc.tuples import *
from rtc.utils import *


class TestTransform(unittest.TestCase):
    def test_translation(self):
        transform = Transform().translation(5, -3, 2)
        p = Point(-3, 4, 5)
        self.assertTrue(tuple4_eq(transform @ p, Point(2, 1, 7)))

    def test_translation_inverse(self):
        transform = Transform().translation(5, -3, 2).inverse()
        p = Point(-3, 4, 5)
        self.assertTrue(tuple4_eq(transform @ p, Point(-8, 7, 3)))

    def test_translation_vector(self):
        transform = Transform().translation(5, -3, 2)
        v = Vector(-3, 4, 5)
        self.assertTrue(tuple4_eq(transform @ v, v))

    def test_scaling_point(self):
        transform = Transform().scaling(2, 3, 4)
        p = Point(-4, 6, 8)
        self.assertTrue(tuple4_eq(transform @ p, Point(-8, 18, 32)))

    def test_scaling_vector(self):
        transform = Transform().scaling(2, 3, 4)
        p = Vector(-4, 6, 8)
        self.assertTrue(tuple4_eq(transform @ p, Vector(-8, 18, 32)))

    def test_scaling_inverse(self):
        transform = Transform().scaling(2, 3, 4).inverse()
        p = Vector(-4, 6, 8)
        self.assertTrue(tuple4_eq(transform @ p, Vector(-2, 2, 2)))

    def test_scaling_reflection(self):
        transform = Transform().scaling(-1, 1, 1).inverse()
        p = Point(2, 3, 4)
        self.assertTrue(tuple4_eq(transform @ p, Point(-2, 3, 4)))

    def test_rotation_x(self):
        p = Point(0, 1, 0)
        half_quarter = Transform().rotation_x(math.pi / 4)
        full_quarter = Transform().rotation_x(math.pi / 2)

        self.assertTrue(tuple4_eq(half_quarter @ p, Point(0, math.sqrt(2) / 2, math.sqrt(2) / 2)))
        self.assertTrue(tuple4_eq(full_quarter @ p, Point(0, 0, 1)))
        self.assertTrue(tuple4_eq(
            half_quarter.inverse() @ p, Point(0, math.sqrt(2) / 2, -math.sqrt(2) / 2)
        ))

    def test_rotation_y(self):
        p = Point(0, 0, 1)
        half_quarter = Transform().rotation_y(math.pi / 4)
        full_quarter = Transform().rotation_y(math.pi / 2)

        self.assertTrue(tuple4_eq(half_quarter @ p, Point(math.sqrt(2) / 2, 0, math.sqrt(2) / 2)))
        self.assertTrue(tuple4_eq(full_quarter @ p, Point(1, 0, 0)))

    def test_rotation_z(self):
        p = Point(0, 1, 0)
        half_quarter = Transform().rotation_z(math.pi / 4)
        full_quarter = Transform().rotation_z(math.pi / 2)

        self.assertTrue(tuple4_eq(
            half_quarter @ p, Point(-math.sqrt(2) / 2, math.sqrt(2) / 2, 0)
        ))
        self.assertTrue(tuple4_eq(full_quarter @ p, Point(-1, 0, 0)))

    def test_shearing(self):
        transform = Transform().shearing(1, 0, 0, 0, 0, 0)
        p = Point(2, 3, 4)
        self.assertTrue(tuple4_eq(transform @ p, Point(5, 3, 4)))

        transform = Transform().shearing(0, 1, 0, 0, 0, 0)
        self.assertTrue(tuple4_eq(transform @ p, Point(6, 3, 4)))

        transform = Transform().shearing(0, 0, 1, 0, 0, 0)
        self.assertTrue(tuple4_eq(transform @ p, Point(2, 5, 4)))

        transform = Transform().shearing(0, 0, 0, 1, 0, 0)
        self.assertTrue(tuple4_eq(transform @ p, Point(2, 7, 4)))

        transform = Transform().shearing(0, 0, 0, 0, 1, 0)
        self.assertTrue(tuple4_eq(transform @ p, Point(2, 3, 6)))

        transform = Transform().shearing(0, 0, 0, 0, 0, 1)
        self.assertTrue(tuple4_eq(transform @ p, Point(2, 3, 7)))

    def test_individual_chaining(self):
        p = Point(1, 0, 1)
        A = Transform().rotation_x(math.pi / 2)
        B = Transform().scaling(5, 5, 5)
        C = Transform().translation(10, 5, 7)

        p2 = A @ p
        self.assertTrue(tuple4_eq(p2, Point(1, -1, 0)))

        p3 = B @ p2
        self.assertTrue(tuple4_eq(p3, Point(5, -5, 0)))

        p4 = C @ p3
        self.assertTrue(tuple4_eq(p4, Point(15, 0, 7)))

    def test_group_chaining(self):
        p = Point(1, 0, 1)
        A = Transform().rotation_x(math.pi / 2)
        B = Transform().scaling(5, 5, 5)
        C = Transform().translation(10, 5, 7)
        T = C @ B @ A

        self.assertTrue(tuple4_eq(T @ p, Point(15, 0, 7)))

        # pylint: disable=no-member
        T2 = Transform().rotation_x(math.pi / 2).scaling(5, 5, 5).translation(10, 5, 7)
        self.assertEqual(T, T2)
        self.assertTrue(tuple4_eq(T2 @ p, Point(15, 0, 7)))

    def test_default_view_transform(self):
        at = Point(0, 0, 0)
        to = Point(0, 0, -1)
        up = Vector(0, 1, 0)
        t = ViewTransform(at, to, up)

        self.assertEqual(t, Transform())

    def test_view_transform_z(self):
        at = Point(0, 0, 0)
        to = Point(0, 0, 1)
        up = Vector(0, 1, 0)
        t = ViewTransform(at, to, up)

        self.assertEqual(t, Transform().scaling(-1, 1, -1))

    def test_view_transform_moves_world(self):
        at = Point(0, 0, 8)
        to = Point(0, 0, 0)
        up = Vector(0, 1, 0)
        t = ViewTransform(at, to, up)

        self.assertEqual(t, Transform().translation(0, 0, -8))

    def test_view_transform_arbitrary(self):
        at = Point(1, 3, 2)
        to = Point(4, -2, 8)
        up = Vector(1, 1, 0)
        t = ViewTransform(at, to, up)

        expected = Matrix(
                -0.50709, 0.50709, 0.67612, -2.36643,
                0.76772, 0.60609, 0.12122, -2.82843,
                -0.35857, 0.59761, -0.71714, 0.00000,
                0.00000, 0.00000, 0.00000, 1.00000,
        )

        self.assertEqual(t, expected)
