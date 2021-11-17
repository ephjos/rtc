import unittest

from rtc.plane import Plane
from rtc.ray import Ray
from rtc.tuples import Point, Vector


class TestPlane(unittest.TestCase):
    def test_plane_normal_constant(self):
        p = Plane()
        n1 = p.local_normal_at(Point(0, 0, 0))
        n2 = p.local_normal_at(Point(10, 0, -10))
        n3 = p.local_normal_at(Point(-5, 0, 150))
        self.assertEqual(n1, Vector(0, 1, 0))
        self.assertEqual(n2, Vector(0, 1, 0))
        self.assertEqual(n3, Vector(0, 1, 0))

    def test_ray_intersect_parallel(self):
        p = Plane()
        r = Ray(Point(0, 10, 0), Vector(0, 0, 1))
        xs = p.local_intersect(r)
        self.assertEqual(len(xs), 0)

    def test_ray_intersect_coplanar(self):
        p = Plane()
        r = Ray(Point(0, 0, 0), Vector(0, 0, 1))
        xs = p.local_intersect(r)
        self.assertEqual(len(xs), 0)

    def test_ray_intersect_above(self):
        p = Plane()
        r = Ray(Point(0, 1, 0), Vector(0, -1, 0))
        xs = p.local_intersect(r)
        self.assertEqual(len(xs), 1)
        self.assertEqual(xs[0].t, 1)
        self.assertEqual(xs[0].object, p)

    def test_ray_intersect_below(self):
        p = Plane()
        r = Ray(Point(0, -1, 0), Vector(0, 1, 0))
        xs = p.local_intersect(r)
        self.assertEqual(len(xs), 1)
        self.assertEqual(xs[0].t, 1)
        self.assertEqual(xs[0].object, p)
