import unittest

from rtc.color import Color
from rtc.pattern import (
    Pattern,
    CheckersPattern,
    GradientPattern,
    RingPattern,
    StripePattern,
)
from rtc.sphere import Sphere
from rtc.matrix import *
from rtc.transform import *
from rtc.tuples import Point

black = Color(0, 0, 0)
white = Color(1, 1, 1)


class TestPattern(unittest.TestCase):
    def test_create_stripe(self):
        p = StripePattern(white, black)
        self.assertEqual(p.a, white)
        self.assertEqual(p.b, black)

    def test_stripe_constant_in_y(self):
        p = StripePattern(white, black)
        self.assertEqual(p.pattern_at(Point(0, 0, 0)), white)
        self.assertEqual(p.pattern_at(Point(0, 1, 0)), white)
        self.assertEqual(p.pattern_at(Point(0, 2, 0)), white)

    def test_stripe_constant_in_z(self):
        p = StripePattern(white, black)
        self.assertEqual(p.pattern_at(Point(0, 0, 0)), white)
        self.assertEqual(p.pattern_at(Point(0, 0, 1)), white)
        self.assertEqual(p.pattern_at(Point(0, 0, 2)), white)

    def test_stripe_alternates_in_z(self):
        p = StripePattern(white, black)
        self.assertEqual(p.pattern_at(Point(0, 0, 0)), white)
        self.assertEqual(p.pattern_at(Point(0.9, 0, 0)), white)
        self.assertEqual(p.pattern_at(Point(1, 0, 0)), black)
        self.assertEqual(p.pattern_at(Point(-0.1, 0, 0)), black)
        self.assertEqual(p.pattern_at(Point(-1, 0, 0)), black)
        self.assertEqual(p.pattern_at(Point(-1.1, 0, 0)), white)

    def test_stripes_with_shape_transformation(self):
        o = Sphere()
        o.transform = scaling(2, 2, 2)
        p = StripePattern(white, black)
        c = p.pattern_at_shape(o, Point(1.5, 0, 0))
        self.assertEqual(c, white)

    def test_stripes_with_pattern_transformation(self):
        o = Sphere()
        p = StripePattern(white, black)
        p.transform = scaling(2, 2, 2)
        c = p.pattern_at_shape(o, Point(1.5, 0, 0))
        self.assertEqual(c, white)

    def test_stripes_with_both_transformation(self):
        o = Sphere()
        o.transform = scaling(2, 2, 2)
        p = StripePattern(white, black)
        p.transform = translation(0.5, 0, 0)
        c = p.pattern_at_shape(o, Point(2.5, 0, 0))
        self.assertEqual(c, white)

    def test_pattern_default_transformation(self):
        p = Pattern()
        self.assertEqual(p.transform, IdentityMatrix())

    def test_pattern_assign_transformation(self):
        p = Pattern()
        p.transform = translation(1, 2, 3)
        self.assertEqual(p.transform, translation(1, 2, 3))

    def test_pattern_shape_transformation(self):
        s = Sphere()
        s.transform = scaling(2, 2, 2)
        p = Pattern()
        c = p.pattern_at_shape(s, Point(2, 3, 4))
        self.assertEqual(c, Color(1, 1.5, 2))

    def test_pattern_pattern_transformation(self):
        s = Sphere()
        p = Pattern()
        p.transform = scaling(2, 2, 2)
        c = p.pattern_at_shape(s, Point(2, 3, 4))
        self.assertEqual(c, Color(1, 1.5, 2))

    def test_pattern_both_transformation(self):
        s = Sphere()
        s.transform = scaling(2, 2, 2)
        p = Pattern()
        p.transform = translation(0.5, 1, 1.5)
        c = p.pattern_at_shape(s, Point(2.5, 3, 3.5))
        self.assertEqual(c, Color(0.75, 0.5, 0.25))

    def test_pattern_gradient(self):
        p = GradientPattern(white, black)
        self.assertEqual(p.pattern_at(Point(0, 0, 0)), white)
        self.assertEqual(p.pattern_at(Point(0.25, 0, 0)), Color(0.75, 0.75, 0.75))
        self.assertEqual(p.pattern_at(Point(0.5, 0, 0)), Color(0.5, 0.5, 0.5))
        self.assertEqual(p.pattern_at(Point(0.75, 0, 0)), Color(0.25, 0.25, 0.25))

    def test_pattern_ring(self):
        p = RingPattern(white, black)
        self.assertEqual(p.pattern_at(Point(0, 0, 0)), white)
        self.assertEqual(p.pattern_at(Point(1, 0, 0)), black)
        self.assertEqual(p.pattern_at(Point(0, 0, 1)), black)
        self.assertEqual(p.pattern_at(Point(0.708, 0, 0.708)), black)

    def test_pattern_checkers_x(self):
        p = CheckersPattern(white, black)
        self.assertEqual(p.pattern_at(Point(0, 0, 0)), white)
        self.assertEqual(p.pattern_at(Point(0.99, 0, 0)), white)
        self.assertEqual(p.pattern_at(Point(1.01, 0, 0)), black)

    def test_pattern_checkers_y(self):
        p = CheckersPattern(white, black)
        self.assertEqual(p.pattern_at(Point(0, 0, 0)), white)
        self.assertEqual(p.pattern_at(Point(0, 0.99, 0)), white)
        self.assertEqual(p.pattern_at(Point(0, 1.01, 0)), black)

    def test_pattern_checkers_z(self):
        p = CheckersPattern(white, black)
        self.assertEqual(p.pattern_at(Point(0, 0, 0)), white)
        self.assertEqual(p.pattern_at(Point(0, 0, 0.99)), white)
        self.assertEqual(p.pattern_at(Point(0, 0, 1.01)), black)
