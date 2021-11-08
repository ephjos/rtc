import unittest

from rtc.color import Color


class TestColor(unittest.TestCase):
    def test_color_constructor(self):
        c = Color(-0.5, 0.4, 1.7)
        self.assertEqual(c.r, -0.5)
        self.assertEqual(c.g, 0.4)
        self.assertEqual(c.b, 1.7)

    def test_add_color(self):
        c1 = Color(0.9, 0.6, 0.75)
        c2 = Color(0.7, 0.1, 0.25)
        self.assertEqual(c1 + c2, Color(1.6, 0.7, 1.0))

    def test_sub_color(self):
        c1 = Color(0.9, 0.6, 0.75)
        c2 = Color(0.7, 0.1, 0.25)
        self.assertEqual(c1 - c2, Color(0.2, 0.5, 0.5))

    def test_mul_color(self):
        c = Color(0.2, 0.3, 0.4)
        self.assertEqual(c * 2, Color(0.4, 0.6, 0.8))

    def test_blend_color(self):
        c1 = Color(1, 0.2, 0.4)
        c2 = Color(0.9, 1, 0.1)
        self.assertEqual(c1.blend(c2), Color(0.9, 0.2, 0.04))
