import math
import unittest

from rtc.camera import Camera
from rtc.color import Color
from rtc.transform import Transform, ViewTransform
from rtc.tuples import *
from rtc.utils import req
from rtc.world import DefaultWorld


class TestCamera(unittest.TestCase):
    def test_camera_constructor(self):
        hsize = 160
        vsize = 120
        fov = math.pi / 2
        c = Camera(hsize, vsize, fov)

        self.assertEqual(c.hsize, 160)
        self.assertEqual(c.vsize, 120)
        self.assertEqual(c.fov, math.pi / 2)
        self.assertEqual(c.transform, Transform())

    def test_camera_horizontal_pixel_size(self):
        c = Camera(200, 125, math.pi / 2)
        self.assertTrue(req(c.pixel_size, 0.01))

    def test_camera_vertical_pixel_size(self):
        c = Camera(125, 200, math.pi / 2)
        self.assertTrue(req(c.pixel_size, 0.01))

    def test_ray_through_center(self):
        c = Camera(201, 101, math.pi / 2)
        r = c.ray_for_pixel(100, 50)
        self.assertTrue(tuple4_eq(r.origin, Point(0, 0, 0)))
        self.assertTrue(tuple4_eq(r.direction, Vector(0, 0, -1)))

    def test_ray_through_corner(self):
        c = Camera(201, 101, math.pi / 2)
        r = c.ray_for_pixel(0, 0)
        self.assertTrue(tuple4_eq(r.origin, Point(0, 0, 0)))
        self.assertTrue(tuple4_eq(r.direction, Vector(0.66519, 0.33259, -0.66851)))

    def test_ray_when_transformed(self):
        c = Camera(201, 101, math.pi / 2)
        c.transform = Transform().translation(0, -2, 5).rotation_y(math.pi / 4)
        r = c.ray_for_pixel(100, 50)
        self.assertTrue(tuple4_eq(r.direction, Vector(math.sqrt(2) / 2, 0, -math.sqrt(2) / 2)))
        self.assertTrue(tuple4_eq(r.origin, Point(0, 2, -5)))

    def test_camera_render(self):
        w = DefaultWorld()
        c = Camera(11, 11, math.pi / 2)
        at = Point(0, 0, -5)
        to = Point(0, 0, 0)
        up = Vector(0, 1, 0)
        c.transform = ViewTransform(at, to, up)
        image = c.render(w)
        self.assertEqual(image.read(5, 5), Color(0.38066, 0.47583, 0.2855))
