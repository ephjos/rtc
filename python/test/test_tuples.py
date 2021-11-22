import math
import unittest

from rtc.tuples import *
from rtc.utils import *


class TestTuple4s(unittest.TestCase):
    def test_tuple_w_1(self):
        a = Tuple4(4.3, -4.2, 3.1, 1.0)
        self.assertTrue(req(a[0], 4.3))
        self.assertTrue(req(a[1], -4.2))
        self.assertTrue(req(a[2], 3.1))
        self.assertTrue(req(a[3], 1.0))
        self.assertTrue(tuple4_is_point(a))
        self.assertFalse(tuple4_is_vector(a))

    def test_tuple_w_0(self):
        a = Tuple4(4.3, -4.2, 3.1, 0.0)
        self.assertTrue(req(a[0], 4.3))
        self.assertTrue(req(a[1], -4.2))
        self.assertTrue(req(a[2], 3.1))
        self.assertTrue(req(a[3], 0.0))
        self.assertFalse(tuple4_is_point(a))
        self.assertTrue(tuple4_is_vector(a))

    def test_point_creates(self):
        p = Point(4, -4, 3)
        self.assertTrue(tuple4_eq(p, Tuple4(4, -4, 3, 1)))

    def test_vector_creates(self):
        p = Vector(4, -4, 3)
        self.assertTrue(tuple4_eq(p, Tuple4(4, -4, 3, 0)))

    def test_add_tuples(self):
        a1 = Tuple4(3, -2, 5, 1)
        a2 = Tuple4(-2, 3, 1, 0)
        self.assertTrue(tuple4_eq(tuple4_add(a1,a2), Tuple4(1, 1, 6, 1)))

    def test_sub_points(self):
        p1 = Point(3, 2, 1)
        p2 = Point(5, 6, 7)
        self.assertTrue(tuple4_eq(tuple4_sub(p1, p2), Vector(-2, -4, -6)))

    def test_sub_point_vector(self):
        p = Point(3, 2, 1)
        v = Vector(5, 6, 7)
        self.assertTrue(tuple4_eq(tuple4_sub(p, v), Point(-2, -4, -6)))

    def test_sub_vector(self):
        v1 = Vector(3, 2, 1)
        v2 = Vector(5, 6, 7)
        self.assertTrue(tuple4_eq(tuple4_sub(v1, v2), Vector(-2, -4, -6)))

    def test_sub_zero(self):
        zero = Vector(0, 0, 0)
        v = Vector(1, -2, 3)
        self.assertTrue(tuple4_eq(tuple4_sub(zero, v), Vector(-1, 2, -3)))

    def test_neg_tuple(self):
        a = Tuple4(1, -2, 3, -4)
        self.assertTrue(tuple4_eq(tuple4_neg(a), Tuple4(-1, 2, -3, 4)))

    def test_scale(self):
        a = Tuple4(1, -2, 3, -4)
        self.assertTrue(tuple4_eq(tuple4_scale(a, 3.5), Tuple4(3.5, -7, 10.5, -14)))
        self.assertTrue(tuple4_eq(tuple4_scale(a, 0.5), Tuple4(0.5, -1, 1.5, -2)))
        self.assertTrue(tuple4_eq(tuple4_divide(a, 2), Tuple4(0.5, -1, 1.5, -2)))

    def test_magnitude(self):
        v = Vector(1, 0, 0)
        self.assertTrue(req(tuple4_magnitude(v), 1))

        v = Vector(0, 1, 0)
        self.assertTrue(req(tuple4_magnitude(v), 1))

        v = Vector(0, 0, 1)
        self.assertTrue(req(tuple4_magnitude(v), 1))

        v = Vector(1, 2, 3)
        self.assertTrue(req(tuple4_magnitude(v), math.sqrt(14)))

        v = Vector(-1, -2, -3)
        self.assertTrue(req(tuple4_magnitude(v), math.sqrt(14)))

    def test_normalize(self):
        v = Vector(4, 0, 0)
        self.assertTrue(tuple4_eq(tuple4_normalize(v), Vector(1, 0, 0)))

        v = Vector(1, 2, 3)
        self.assertEqual(
            tuple4_normalize(v),
            Vector(1 / math.sqrt(14), 2 / math.sqrt(14), 3 / math.sqrt(14)),
        )
        self.assertTrue(req(tuple4_magnitude(tuple4_normalize(v)), 1))

    def test_dot(self):
        a = Vector(1, 2, 3)
        b = Vector(2, 3, 4)
        self.assertTrue(req(tuple4_dot(a,b), 20))

    def test_cross(self):
        a = Vector(1, 2, 3)
        b = Vector(2, 3, 4)
        self.assertTrue(tuple4_eq(tuple4_cross(a,b), Vector(-1, 2, -1)))
        self.assertTrue(tuple4_eq(tuple4_cross(b,a), Vector(1, -2, 1)))

    def test_reflect_45(self):
        v = Vector(1, -1, 0)
        n = Vector(0, 1, 0)
        r = tuple4_reflect(v, n)
        self.assertTrue(tuple4_eq(r, Vector(1, 1, 0)))

    def test_reflect_slanted(self):
        v = Vector(0, -1, 0)
        n = Vector(math.sqrt(2) / 2, math.sqrt(2) / 2, 0)
        r = tuple4_reflect(v, n)
        self.assertTrue(tuple4_eq(r, Vector(1, 0, 0)))
