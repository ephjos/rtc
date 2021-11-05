#!/usr/bin/env python

import math
import unittest

from transform import Transform
from tuples import Tuple, Point, Vector

class Ray:
    def __init__(self, origin, direction):
        if not origin.isPoint():
            raise Exception("origin must be a point")
        if not direction.isVector():
            raise Exception("direction must be a vector")
        self.origin = origin
        self.direction = direction

    def position(self, t):
        return self.origin + self.direction*t

    def transform(self, t):
        origin = t @ self.origin
        direction = t @ self.direction
        return Ray(origin, direction)

class TestRay(unittest.TestCase):
    def test_ray_constructor(self):
        origin = Point(1,2,3)
        direction = Vector(4,5,6)
        r = Ray(origin, direction)

        self.assertEqual(r.origin, origin)
        self.assertEqual(r.direction, direction)

    def test_ray_position(self):
        r = Ray(Point(2,3,4), Vector(1,0,0))

        self.assertEqual(r.position(0), Point(2,3,4))
        self.assertEqual(r.position(1), Point(3,3,4))
        self.assertEqual(r.position(-1), Point(1,3,4))
        self.assertEqual(r.position(2.5), Point(4.5,3,4))

    def test_ray_translate(self):
        r = Ray(Point(1,2,3), Vector(0,1,0))
        m = Transform().translation(3,4,5)

        r2 = r.transform(m)
        self.assertEqual(r2.origin, Point(4,6,8))
        self.assertEqual(r2.direction, Vector(0,1,0))

    def test_ray_scale(self):
        r = Ray(Point(1,2,3), Vector(0,1,0))
        m = Transform().scaling(2,3,4)

        r2 = r.transform(m)
        self.assertEqual(r2.origin, Point(2,6,12))
        self.assertEqual(r2.direction, Vector(0,3,0))

