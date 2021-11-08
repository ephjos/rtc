import math
import unittest

from rtc.tuples import Tuple, Vector, Point


class TestTuples(unittest.TestCase):
    def test_tuple_w_1(self):
        a = Tuple(4.3, -4.2, 3.1, 1.0)
        self.assertEqual(a.x, 4.3)
        self.assertEqual(a.y, -4.2)
        self.assertEqual(a.z, 3.1)
        self.assertEqual(a.w, 1.0)
        self.assertTrue(a.isPoint())
        self.assertFalse(a.isVector())

    def test_tuple_w_0(self):
        a = Tuple(4.3, -4.2, 3.1, 0.0)
        self.assertEqual(a.x, 4.3)
        self.assertEqual(a.y, -4.2)
        self.assertEqual(a.z, 3.1)
        self.assertEqual(a.w, 0.0)
        self.assertFalse(a.isPoint())
        self.assertTrue(a.isVector())

    def test_point_creates(self):
        p = Point(4, -4, 3)
        self.assertEqual(p, Tuple(4, -4, 3, 1))

    def test_vector_creates(self):
        p = Vector(4, -4, 3)
        self.assertEqual(p, Tuple(4, -4, 3, 0))

    def test_add_tuples(self):
        a1 = Tuple(3, -2, 5, 1)
        a2 = Tuple(-2, 3, 1, 0)
        self.assertEqual(a1 + a2, Tuple(1, 1, 6, 1))

    def test_sub_points(self):
        p1 = Point(3, 2, 1)
        p2 = Point(5, 6, 7)
        self.assertEqual(p1 - p2, Vector(-2, -4, -6))

    def test_sub_point_vector(self):
        p = Point(3, 2, 1)
        v = Vector(5, 6, 7)
        self.assertEqual(p - v, Point(-2, -4, -6))

    def test_sub_vector(self):
        v1 = Vector(3, 2, 1)
        v2 = Vector(5, 6, 7)
        self.assertEqual(v1 - v2, Vector(-2, -4, -6))

    def test_sub_zero(self):
        zero = Vector(0, 0, 0)
        v = Vector(1, -2, 3)
        self.assertEqual(zero - v, Vector(-1, 2, -3))

    def test_neg_tuple(self):
        a = Tuple(1, -2, 3, -4)
        self.assertEqual(-a, Tuple(-1, 2, -3, 4))

    def test_scale(self):
        a = Tuple(1, -2, 3, -4)
        self.assertEqual(a * 3.5, Tuple(3.5, -7, 10.5, -14))
        self.assertEqual(a * 0.5, Tuple(0.5, -1, 1.5, -2))
        self.assertEqual(a / 2, Tuple(0.5, -1, 1.5, -2))

    def test_magnitude(self):
        v = Vector(1, 0, 0)
        self.assertEqual(v.magnitude(), 1)

        v = Vector(0, 1, 0)
        self.assertEqual(v.magnitude(), 1)

        v = Vector(0, 0, 1)
        self.assertEqual(v.magnitude(), 1)

        v = Vector(1, 2, 3)
        self.assertEqual(v.magnitude(), math.sqrt(14))

        v = Vector(-1, -2, -3)
        self.assertEqual(v.magnitude(), math.sqrt(14))

    def test_normalize(self):
        v = Vector(4, 0, 0)
        self.assertEqual(v.normalize(), Vector(1, 0, 0))

        v = Vector(1, 2, 3)
        self.assertEqual(
            v.normalize(),
            Vector(1 / math.sqrt(14), 2 / math.sqrt(14), 3 / math.sqrt(14)),
        )
        self.assertEqual(v.normalize().magnitude(), 1)

    def test_dot(self):
        a = Vector(1, 2, 3)
        b = Vector(2, 3, 4)
        self.assertEqual(a.dot(b), 20)

    def test_cross(self):
        a = Vector(1, 2, 3)
        b = Vector(2, 3, 4)
        self.assertEqual(a.cross(b), Vector(-1, 2, -1))
        self.assertEqual(b.cross(a), Vector(1, -2, 1))

    def test_reflect_45(self):
        v = Vector(1, -1, 0)
        n = Vector(0, 1, 0)
        r = v.reflect(n)
        self.assertEqual(r, Vector(1, 1, 0))

    def test_reflect_slanted(self):
        v = Vector(0, -1, 0)
        n = Vector(math.sqrt(2) / 2, math.sqrt(2) / 2, 0)
        r = v.reflect(n)
        self.assertEqual(r, Vector(1, 0, 0))
