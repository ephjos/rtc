import math
import unittest

from rtc.intersection import Intersections, Intersection
from rtc.plane import Plane
from rtc.ray import Ray
from rtc.sphere import Sphere, GlassSphere
from rtc.transform import Transform
from rtc.tuples import Point, Vector
from rtc.utils import EPSILON, req


class TestIntersection(unittest.TestCase):
    def test_prepare_computations(self):
        r = Ray(Point(0, 0, -5), Vector(0, 0, 1))
        shape = Sphere()
        i = Intersection(4, shape)

        comps = i.prepare_computations(r, Intersections([i]))

        self.assertEqual(comps.t, i.t)
        self.assertEqual(comps.shape, i.shape)
        self.assertEqual(comps.point, Point(0, 0, -1))
        self.assertEqual(comps.eyev, Vector(0, 0, -1))
        self.assertEqual(comps.normalv, Vector(0, 0, -1))

    def test_prepare_computations_not_inside(self):
        r = Ray(Point(0, 0, -5), Vector(0, 0, 1))
        shape = Sphere()
        i = Intersection(4, shape)

        comps = i.prepare_computations(r, Intersections([i]))

        self.assertFalse(comps.inside)

    def test_prepare_computations_inside(self):
        r = Ray(Point(0, 0, 0), Vector(0, 0, 1))
        shape = Sphere()
        i = Intersection(1, shape)

        comps = i.prepare_computations(r, Intersections([i]))

        self.assertEqual(comps.point, Point(0, 0, 1))
        self.assertEqual(comps.eyev, Vector(0, 0, -1))
        self.assertTrue(comps.inside)
        self.assertEqual(comps.normalv, Vector(0, 0, -1))

    def test_hit_offset_the_point(self):
        r = Ray(Point(0, 0, -5), Vector(0, 0, 1))
        shape = Sphere()
        shape.transform = Transform().translation(0, 0, 1)
        i = Intersection(5, shape)
        comps = i.prepare_computations(r, Intersections([i]))

        self.assertLess(comps.over_point.z, -EPSILON / 2)
        self.assertGreater(comps.point.z, comps.over_point.z)

    def test_precompute_reflect(self):
        shape = Plane()
        ray = Ray(Point(0, 1, -1), Vector(0, -math.sqrt(2) / 2, math.sqrt(2) / 2))
        i = Intersection(math.sqrt(2), shape)
        comps = i.prepare_computations(ray, Intersections([i]))
        self.assertEqual(comps.reflectv, Vector(0, math.sqrt(2) / 2, math.sqrt(2) / 2))

    def test_prepare_computations_refractive(self):
        examples = [
            [0, 1.0, 1.5],
            [1, 1.5, 2.0],
            [2, 2.0, 2.5],
            [3, 2.5, 2.5],
            [4, 2.5, 1.5],
            [5, 1.5, 1.0],
        ]

        A = GlassSphere()
        A.transform = Transform().scaling(2, 2, 2)
        A.material.refractive_index = 1.5

        B = GlassSphere()
        B.transform = Transform().translation(0, 0, -0.25)
        B.material.refractive_index = 2.0

        C = GlassSphere()
        C.transform = Transform().translation(0, 0, 0.25)
        C.material.refractive_index = 2.5

        r = Ray(Point(0, 0, -4), Vector(0, 0, 1))
        xs = Intersections(
            [
                Intersection(2, A),
                Intersection(2.75, B),
                Intersection(3.25, C),
                Intersection(4.75, B),
                Intersection(5.25, C),
                Intersection(6, A),
            ]
        )

        for index, n1, n2 in examples:
            comps = xs[index].prepare_computations(r, xs)
            self.assertEqual(comps.n1, n1, msg="Index " + str(index))
            self.assertEqual(comps.n2, n2, msg="Index " + str(index))

    def test_prepare_computations_under_point(self):
        r = Ray(Point(0, 0, -5), Vector(0, 0, 1))
        shape = GlassSphere()
        shape.transform = Transform().translation(0, 0, 1)
        i = Intersection(5, shape)
        xs = Intersections([i])
        comps = i.prepare_computations(r, xs)
        self.assertGreater(comps.under_point.z, EPSILON / 2)
        self.assertLess(comps.point.z, comps.under_point.z)

    def test_intersection_slick_total_internal_reflection(self):
        shape = GlassSphere()
        r = Ray(Point(0,0,math.sqrt(2)/2), Vector(0,1,0))
        xs = Intersections([Intersection(-math.sqrt(2)/2,shape),Intersection(math.sqrt(2)/2,shape)])
        comps = xs[1].prepare_computations(r, xs)
        reflectance = comps.schlick()
        self.assertEqual(reflectance, 1.0)

    def test_intersection_slick_perpendicular(self):
        shape = GlassSphere()
        r = Ray(Point(0,0,0), Vector(0,1,0))
        xs = Intersections([
            Intersection(-1,shape),
            Intersection(1,shape)
        ])
        comps = xs[1].prepare_computations(r, xs)
        reflectance = comps.schlick()
        self.assertTrue(req(reflectance, 0.04))

    def test_intersection_slick_small_angle(self):
        shape = GlassSphere()
        r = Ray(Point(0,0.99,-2), Vector(0,0,1))
        xs = Intersections([
            Intersection(1.8589,shape),
        ])
        comps = xs[0].prepare_computations(r, xs)
        reflectance = comps.schlick()
        self.assertTrue(req(reflectance, 0.48873))

