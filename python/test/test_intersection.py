
import unittest

from rtc.intersection import Intersection
from rtc.ray import Ray
from rtc.sphere import Sphere
from rtc.transform import Transform
from rtc.tuples import Point, Vector
from rtc.utils import EPSILON

class TestIntersection(unittest.TestCase):
    def test_prepare_computations(self):
        r = Ray(Point(0,0,-5), Vector(0,0,1))
        shape = Sphere()
        i = Intersection(4, shape)

        comps = i.prepare_computations(r)

        self.assertEqual(comps.t, i.t)
        self.assertEqual(comps.object, i.object)
        self.assertEqual(comps.point, Point(0,0,-1))
        self.assertEqual(comps.eyev, Vector(0,0,-1))
        self.assertEqual(comps.normalv, Vector(0,0,-1))

    def test_prepare_computations_not_inside(self):
        r = Ray(Point(0,0,-5), Vector(0,0,1))
        shape = Sphere()
        i = Intersection(4, shape)

        comps = i.prepare_computations(r)

        self.assertFalse(comps.inside)

    def test_prepare_computations_inside(self):
        r = Ray(Point(0,0,0), Vector(0,0,1))
        shape = Sphere()
        i = Intersection(1, shape)

        comps = i.prepare_computations(r)

        self.assertEqual(comps.point, Point(0,0,1))
        self.assertEqual(comps.eyev, Vector(0,0,-1))
        self.assertTrue(comps.inside)
        self.assertEqual(comps.normalv, Vector(0,0,-1))

    def test_hit_offset_the_point(self):
        r = Ray(Point(0, 0, -5), Vector(0, 0, 1))
        shape = Sphere()
        shape.transform = Transform().translation(0, 0, 1)
        i = Intersection(5, shape)
        comps = i.prepare_computations(r)

        self.assertLess(comps.over_point.z, -EPSILON/2)
        self.assertGreater(comps.point.z, comps.over_point.z)
