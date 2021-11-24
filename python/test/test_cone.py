import math
import unittest

from rtc.cone import Cone
from rtc.materials import Material
from rtc.ray import Ray
from rtc.shape import Shape
from rtc.matrix import *
from rtc.transform import *
from rtc.tuples import *
from rtc.utils import *

out_ray = Ray(Point(0, 0, 0), Vector(0, 0, 0))


class TestCone(unittest.TestCase):
    def test_cone_intersect(self):
        c = Cone()

        examples = [
            [Point(0,0,-5), Vector(0,0,1), 5, 5],
            [Point(0,0,-5), Vector(1,1,1), 8.66025, 8.66025],
            [Point(1,1,-5), Vector(-0.5,-1,1), 4.55006, 49.44994],
        ]

        for i, [origin, direction, t0, t1] in enumerate(examples):
            direction = tuple4_normalize(direction)
            ray = Ray(origin, direction)
            xs = c.local_intersect(ray)
            self.assertEqual(len(xs), 2, "Example " + str(i))
            self.assertTrue(req(xs[0].t, t0), "Example " + str(i))
            self.assertTrue(req(xs[1].t, t1), "Example " + str(i))

    def test_cone_intersect_parallel(self):
        c = Cone()
        direction = tuple4_normalize(Vector(0,1,1))
        ray = Ray(Point(0,0,-1), direction)
        xs = c.local_intersect(ray)
        self.assertEqual(len(xs), 1)
        self.assertTrue(req(xs[0].t, 0.35355))

    def test_cone_intersect_caps(self):
        c = Cone()
        c.minimum = -0.5
        c.maximum = 0.5
        c.closed = True

        examples = [
            [Point(0,0,-5), Vector(0,1,0), 0],
            [Point(0,0,-0.25), Vector(0,1,1), 2],
            [Point(0,0,-0.25), Vector(0,1,0), 4],
        ]

        for i, [origin, direction, count] in enumerate(examples):
            direction = tuple4_normalize(direction)
            ray = Ray(origin, direction)
            xs = c.local_intersect(ray)
            self.assertEqual(len(xs), count, "Example " + str(i))

    def test_cone_normal_at(self):
        c = Cone()

        examples = [
            [Point(0,0,0), Vector(0,0,0)],
            [Point(1,1,1), Vector(1,-math.sqrt(2),1)],
            [Point(-1,-1,0), Vector(-1,1,0)],
        ]

        for i, [point, normal] in enumerate(examples):
            n = c.local_normal_at(point)
            self.assertEqual(n, normal, "Example " + str(i))
