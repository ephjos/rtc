#!/usr/bin/env python
import math
import unittest

from utils import req, clamp

class Color:
    def __init__(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b

    def __str__(self):
        display_color = lambda c: clamp(round(c*255), 0, 255)
        r = display_color(self.r)
        g = display_color(self.g)
        b = display_color(self.b)
        return "{} {} {}".format(r, g, b)

    def __repr__(self):
        return str(self)

    def __eq__(self, o):
        return req(self.r,o.r) and req(self.g,o.g) and req(self.b,o.b)

    def __add__(self, o):
        return Color(
            self.r + o.r,
            self.g + o.g,
            self.b + o.b,
        )

    def __sub__(self, o):
        return Color(
            self.r - o.r,
            self.g - o.g,
            self.b - o.b,
        )

    def __mul__(self, s):
        return Color(
            self.r * s,
            self.g * s,
            self.b * s,
        )

    @staticmethod
    def blend(self, o):
        return Color(
            self.r * o.r,
            self.g * o.g,
            self.b * o.b,
        )


class TestColor(unittest.TestCase):
    def test_color_constructor(self):
        c = Color(-0.5, 0.4, 1.7)
        self.assertEqual(c.r, -0.5)
        self.assertEqual(c.g, 0.4)
        self.assertEqual(c.b, 1.7)

    def test_add_color(self):
        c1 = Color(0.9, 0.6, 0.75)
        c2 = Color (0.7, 0.1, 0.25)
        self.assertEqual(c1+c2, Color(1.6, 0.7, 1.0))

    def test_sub_color(self):
        c1 = Color(0.9, 0.6, 0.75)
        c2 = Color (0.7, 0.1, 0.25)
        self.assertEqual(c1-c2, Color(0.2, 0.5, 0.5))

    def test_mul_color(self):
        c = Color(0.2, 0.3, 0.4)
        self.assertEqual(c*2, Color(0.4, 0.6, 0.8))

    def test_blend_color(self):
        c1 = Color(1, 0.2, 0.4)
        c2 = Color(0.9, 1, 0.1)
        self.assertEqual(Color.blend(c1,c2), Color(0.9, 0.2, 0.04))


