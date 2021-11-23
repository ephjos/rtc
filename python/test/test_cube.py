import unittest

from rtc.cube import Cube
from rtc.ray import Ray
from rtc.tuples import Point, Vector


class TestCube(unittest.TestCase):
    def test_cube_intersect(self):
        c = Cube()

        examples = [
            [Point(5, 0.5, 0), Vector(-1, 0, 0), 4, 6],
            [Point(-5, 0.5, 0), Vector(1, 0, 0), 4, 6],
            [Point(0.5, 5, 0), Vector(0, -1, 0), 4, 6],
            [Point(0.5, -5, 0), Vector(0, 1, 0), 4, 6],
            [Point(0.5, 0, 5), Vector(0, 0, -1), 4, 6],
            [Point(0.5, 0, -5), Vector(0, 0, 1), 4, 6],
            [Point(0, 0.5, 0), Vector(0, 0, 1), -1, 1],
        ]

        for i, [origin, direction, t1, t2] in enumerate(examples):
            r = Ray(origin, direction)
            xs = c.local_intersect(r)
            self.assertEqual(len(xs), 2, "Example " + str(i))
            self.assertEqual(xs[0].t, t1, "Example " + str(i))
            self.assertEqual(xs[1].t, t2, "Example " + str(i))

    def test_cube_no_intersect(self):
        c = Cube()

        examples = [
            [Point(-2, 0, 0), Vector(0.2673, 0.5345, 0.8018)],
            [Point(0, -2, 0), Vector(0.8018, 0.2673, 0.5345)],
            [Point(0, 0, -2), Vector(0.5345, 0.8018, 0.2673)],
            [Point(2, 0, 2), Vector(0, 0, -1)],
            [Point(0, 2, 2), Vector(0, -1, 0)],
            [Point(2, 2, 0), Vector(-1, 0, 0)],
        ]

        for i, [origin, direction] in enumerate(examples):
            r = Ray(origin, direction)
            xs = c.local_intersect(r)
            self.assertEqual(len(xs), 0, "Example " + str(i))

    def test_cube_normal(self):
        c = Cube()

        examples = [
            [Point(1, 0.5, -0.8), Vector(1, 0, 0)],
            [Point(-1, -0.2, 0.9), Vector(-1, 0, 0)],
            [Point(-0.4, 1, -0.1), Vector(0, 1, 0)],
            [Point(0.3, -1, -0.7), Vector(0, -1, 0)],
            [Point(-0.6, 0.3, 1), Vector(0, 0, 1)],
            [Point(0.4, 0.4, -1), Vector(0, 0, -1)],
            [Point(1, 1, 1), Vector(1, 0, 0)],
            [Point(-1, -1, -1), Vector(-1, 0, 0)],
        ]

        for i, [point, normal] in enumerate(examples):
            normal_at = c.local_normal_at(point)
            self.assertEqual(normal_at, normal, "Example " + str(i))
