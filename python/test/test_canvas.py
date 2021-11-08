import unittest

from rtc.canvas import Canvas
from rtc.color import Color


class TestCanvas(unittest.TestCase):
    def test_canvas_constructor(self):
        c = Canvas(10, 20)
        self.assertEqual(c.w, 10)
        self.assertEqual(c.h, 20)
        for p in c.p:
            self.assertEqual(p, Color(0, 0, 0))

    def test_write_to_canvas(self):
        c = Canvas(10, 20)
        red = Color(1, 0, 0)
        c.write(2, 3, red)
        self.assertEqual(c.read(2, 3), red)

    def test_ppm_header(self):
        c = Canvas(5, 3)
        self.assertEqual("\n".join(c.ppm().split("\n")[:4]), "P3\n5 3\n255\n")

    def test_ppm(self):
        c = Canvas(5, 3)
        c.write(0, 0, Color(1.5, 0, 0))
        c.write(2, 1, Color(0, 0.5, 0))
        c.write(4, 2, Color(-0.5, 0, 1))

        ppm = c.ppm().split("\n")
        line1 = ppm[4]
        line2 = ppm[5]
        line3 = ppm[6]

        self.assertEqual(line1, "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0")
        self.assertEqual(line2, "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0")
        self.assertEqual(line3, "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255")

    def test_ppm_end_newline(self):
        c = Canvas(5, 3)
        self.assertEqual(c.ppm()[-1], "\n")
