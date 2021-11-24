import math
import unittest

from rtc.cylinder import Cylinder
from rtc.materials import Material
from rtc.ray import Ray
from rtc.shape import Shape
from rtc.matrix import *
from rtc.transform import *
from rtc.tuples import *
from rtc.utils import *

out_ray = Ray(Point(0, 0, 0), Vector(0, 0, 0))


class TestCylinder(unittest.TestCase):
    def test_cylinder_ray_miss(self):
        cyl = Cylinder()

        examples = [
            [Point(1,0,0), Vector(0,1,0)],
            [Point(0,0,0), Vector(0,1,0)],
            [Point(0,0,-5), Vector(1,1,1)],
        ]

        for i, [origin, direction] in enumerate(examples):
            direction = tuple4_normalize(direction)
            ray = Ray(origin, direction)
            xs = cyl.local_intersect(ray)
            self.assertEqual(len(xs), 0, "Example " + str(i))

    def test_cylinder_ray_hit(self):
        cyl = Cylinder()

        examples = [
            [Point(1,0,-5), Vector(0,0,1), 5, 5],
            [Point(0,0,-5), Vector(0,0,1), 4, 6],
            [Point(0.5,0,-5), Vector(0.1,1,1), 6.80798, 7.08872],
        ]

        for i, [origin, direction, t0, t1] in enumerate(examples):
            direction = tuple4_normalize(direction)
            ray = Ray(origin, direction)
            xs = cyl.local_intersect(ray)
            self.assertEqual(len(xs), 2, "Example " + str(i))
            self.assertTrue(req(xs[0].t, t0), "Example " + str(i))
            self.assertTrue(req(xs[1].t, t1), "Example " + str(i))

    def test_cylinder_normal(self):
        cyl = Cylinder()

        examples = [
            [Point(1,0,0), Vector(1,0,0)],
            [Point(0,5,-1), Vector(0,0,-1)],
            [Point(0,-2,1), Vector(0,0,1)],
            [Point(-1,1,0), Vector(-1,0,0)],
        ]

        for i, [point, normal] in enumerate(examples):
            n = cyl.local_normal_at(point)
            self.assertEqual(n, normal, "Example " + str(i))

    def test_cylinder_default_bounds(self):
        cyl = Cylinder()
        self.assertEqual(cyl.minimum, float("-inf"))
        self.assertEqual(cyl.maximum, float("inf"))

    def test_cylinder_truncated_intersect(self):
        cyl = Cylinder()
        cyl.minimum = 1
        cyl.maximum = 2

        examples = [
            [Point(0,1.5,0), Vector(0.1,1,0), 0],
            [Point(0,3,-5), Vector(0,0,1), 0],
            [Point(0,0,-5), Vector(0,0,1), 0],
            [Point(0,0,-5), Vector(0,0,1), 0],
            [Point(0,2,-5), Vector(0,0,1), 0],
            [Point(0,1.5,-2), Vector(0,0,1), 2],
        ]

        for i, [origin, direction, count] in enumerate(examples):
            direction = tuple4_normalize(direction)
            ray = Ray(origin, direction)
            xs = cyl.local_intersect(ray)
            self.assertEqual(len(xs), count, "Example " + str(i))

    def test_cylinder_default_closed(self):
        cyl = Cylinder()
        self.assertEqual(cyl.closed, False)

    def test_cylinder_intersecting_caps(self):
        cyl = Cylinder()
        cyl.minimum = 1
        cyl.maximum = 2
        cyl.closed = True

        examples = [
            [Point(0,3,0), Vector(0,-1,0), 2],
            [Point(0,3,-2), Vector(0,-1,2), 2],
            [Point(0,4,-2), Vector(0,-1,1), 2],
            [Point(0,0,-2), Vector(0,1,2), 2],
            [Point(0,-1,-2), Vector(0,1,1), 2],
        ]

        for i, [origin, direction, count] in enumerate(examples):
            direction = tuple4_normalize(direction)
            ray = Ray(origin, direction)
            xs = cyl.local_intersect(ray)
            self.assertEqual(len(xs), count, "Example " + str(i))


    def test_cylinder_normal_at_cap(self):
        cyl = Cylinder()
        cyl.minimum = 1
        cyl.maximum = 2
        cyl.closed = True

        examples = [
            [Point(0,1,0), Vector(0,-1,0)],
            [Point(0.5,1,0), Vector(0,-1,0)],
            [Point(0,1,0.5), Vector(0,-1,0)],
            [Point(0,2,0), Vector(0,1,0)],
            [Point(0.5,2,0), Vector(0,1,0)],
            [Point(0,2,0.5), Vector(0,1,0)],
        ]

        for i, [point, normal] in enumerate(examples):
            n = cyl.local_normal_at(point)
            self.assertEqual(n, normal, "Example " + str(i))
