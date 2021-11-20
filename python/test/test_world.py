import math
import unittest

from rtc.color import Color
from rtc.intersection import Intersection, Intersections
from rtc.lights import PointLight
from rtc.pattern import Pattern
from rtc.plane import Plane
from rtc.ray import Ray
from rtc.sphere import Sphere
from rtc.transform import Transform
from rtc.tuples import Point, Vector
from rtc.world import World, DefaultWorld


class TestWorld(unittest.TestCase):
    def test_world_constructor(self):
        w = World([], PointLight(Point(0, 0, 0), Color(0, 0, 0)))
        self.assertEqual(w.shapes, [])
        self.assertEqual(w.light, PointLight(Point(0, 0, 0), Color(0, 0, 0)))

    def test_default_world(self):
        light = PointLight(Point(-10, 10, -10), Color(1, 1, 1))
        s1 = Sphere()
        s1.material.color = Color(0.8, 1.0, 0.6)
        s1.material.diffuse = 0.7
        s1.material.specular = 0.2

        s2 = Sphere()
        s2.transform = Transform().scaling(0.5, 0.5, 0.5)

        w = DefaultWorld()

        self.assertEqual(w.light, light)
        self.assertTrue(s1 in w.shapes)
        self.assertTrue(s2 in w.shapes)

    def test_world_intersect(self):
        w = DefaultWorld()
        r = Ray(Point(0, 0, -5), Vector(0, 0, 1))

        xs = w.intersect(r)

        self.assertEqual(len(xs), 4)
        self.assertEqual(xs[0].t, 4)
        self.assertEqual(xs[1].t, 4.5)
        self.assertEqual(xs[2].t, 5.5)
        self.assertEqual(xs[3].t, 6)

    def test_world_shade_hit(self):
        w = DefaultWorld()
        r = Ray(Point(0, 0, -5), Vector(0, 0, 1))
        shape = w.shapes[0]
        i = Intersection(4, shape)
        comps = i.prepare_computations(r, Intersections([i]))
        c = w.shade_hit(comps)

        self.assertEqual(c, Color(0.38066, 0.47583, 0.2855))

    def test_world_shade_hit_inside(self):
        w = DefaultWorld()
        w.light = PointLight(Point(0, 0.25, 0), Color(1, 1, 1))
        r = Ray(Point(0, 0, 0), Vector(0, 0, 1))
        shape = w.shapes[1]
        i = Intersection(0.5, shape)

        comps = i.prepare_computations(r, Intersections([i]))
        c = w.shade_hit(comps)

        self.assertEqual(c, Color(0.90498, 0.90498, 0.90498))

    def test_color_at_misses(self):
        w = DefaultWorld()
        r = Ray(Point(0, 0, -5), Vector(0, 1, 0))
        c = w.color_at(r)

        self.assertEqual(c, Color(0, 0, 0))

    def test_color_at_hits(self):
        w = DefaultWorld()
        r = Ray(Point(0, 0, -5), Vector(0, 0, 1))
        c = w.color_at(r)

        self.assertEqual(c, Color(0.38066, 0.47583, 0.2855))

    def test_color_at_hits_behind(self):
        w = DefaultWorld()
        outer = w.shapes[0]
        outer.material.ambient = 1
        inner = w.shapes[1]
        inner.material.ambient = 1

        r = Ray(Point(0, 0, 0.75), Vector(0, 0, -1))
        c = w.color_at(r)

        self.assertEqual(c, inner.material.color)

    def test_world_no_shadow(self):
        w = DefaultWorld()
        p = Point(0, 10, 0)

        self.assertFalse(w.is_shadowed(p))

    def test_world_in_shadow(self):
        w = DefaultWorld()
        p = Point(10, -10, 10)

        self.assertTrue(w.is_shadowed(p))

    def test_world_no_shadow_shape_behind_light(self):
        w = DefaultWorld()
        p = Point(-20, 20, -20)

        self.assertFalse(w.is_shadowed(p))

    def test_world_no_shadow_shape_behind_point(self):
        w = DefaultWorld()
        p = Point(-2, 2, -2)

        self.assertFalse(w.is_shadowed(p))

    def test_world_shade_hit_intersection_in_shadow(self):
        w = World([], PointLight(Point(0, 0, 0), Color(0, 0, 0)))
        w.light = PointLight(Point(0, 0, -10), Color(1, 1, 1))
        s1 = Sphere()
        s2 = Sphere()
        s2.transform = Transform().translation(0, 0, 10)
        w.shapes = [s1, s2]
        r = Ray(Point(0, 0, 5), Vector(0, 0, 1))
        i = Intersection(4, s2)
        comps = i.prepare_computations(r, Intersections([i]))
        c = w.shade_hit(comps)

        self.assertEqual(c, Color(0.1, 0.1, 0.1))

    def test_world_reflected_color(self):
        w = DefaultWorld()
        r = Ray(Point(0, 0, 0), Vector(0, 0, 1))
        shape = w.shapes[1]
        shape.material.ambient = 1
        i = Intersection(1, shape)
        comps = i.prepare_computations(r, Intersections([i]))
        color = w.reflected_color(comps)
        self.assertEqual(color, Color(0, 0, 0))

    def test_world_reflected_color_reflective(self):
        w = DefaultWorld()
        shape = Plane()
        shape.material.reflective = 0.5
        shape.transform = Transform().translation(0, -1, 0)
        w.shapes.append(shape)
        r = Ray(Point(0, 0, -3), Vector(0, -math.sqrt(2) / 2, math.sqrt(2) / 2))
        i = Intersection(math.sqrt(2), shape)
        comps = i.prepare_computations(r, Intersections([i]))
        color = w.reflected_color(comps)
        self.assertEqual(color, Color(0.19032, 0.2379, 0.14274))

    def test_world_shade_hit_reflective(self):
        w = DefaultWorld()
        shape = Plane()
        shape.material.reflective = 0.5
        shape.transform = Transform().translation(0, -1, 0)
        w.shapes.append(shape)
        r = Ray(Point(0, 0, -3), Vector(0, -math.sqrt(2) / 2, math.sqrt(2) / 2))
        i = Intersection(math.sqrt(2), shape)
        comps = i.prepare_computations(r, Intersections([i]))
        color = w.shade_hit(comps)
        self.assertEqual(color, Color(0.87677, 0.92436, 0.82918))

    def test_world_mutally_reflective(self):
        w = World([], PointLight(Point(0, 0, 0), Color(0, 0, 0)))
        w.light = PointLight(Point(0, 0, 0), Color(1, 1, 1))
        lower = Plane()
        lower.material.reflective = 1
        lower.transform = Transform().translation(0, -1, 0)
        w.shapes.append(lower)
        upper = Plane()
        upper.material.reflective = 1
        upper.transform = Transform().translation(0, 1, 0)
        w.shapes.append(upper)
        r = Ray(Point(0, 0, 0), Vector(0, 1, 0))

        self.assertIsNotNone(w.color_at(r))

    def test_world_reflective_max_depth(self):
        w = DefaultWorld()
        shape = Plane()
        shape.material.reflective = 0.5
        shape.transform = Transform().translation(0, -1, 0)
        w.shapes.append(shape)
        r = Ray(Point(0, 0, -3), Vector(0, -math.sqrt(2) / 2, math.sqrt(2) / 2))
        i = Intersection(math.sqrt(2), shape)
        comps = i.prepare_computations(r, Intersections([i]))
        color = w.reflected_color(comps, 0)
        self.assertEqual(color, Color(0, 0, 0))

    def test_world_color_refracted_opaque(self):
        w = DefaultWorld()
        shape = w.shapes[0]
        r = Ray(Point(0, 0, -5), Vector(0, 0, 1))
        xs = Intersections(
            [
                Intersection(4, shape),
                Intersection(6, shape),
            ]
        )
        comps = xs[0].prepare_computations(r, xs)
        color = w.refracted_color(comps, 5)
        self.assertEqual(color, Color(0, 0, 0))

    def test_world_color_refracted_max_depth(self):
        w = DefaultWorld()
        shape = w.shapes[0]
        shape.material.transparency = 1.0
        shape.material.refractive_index = 1.5
        r = Ray(Point(0, 0, -5), Vector(0, 0, 1))
        xs = Intersections(
            [
                Intersection(4, shape),
                Intersection(6, shape),
            ]
        )
        comps = xs[0].prepare_computations(r, xs)
        color = w.refracted_color(comps, 0)
        self.assertEqual(color, Color(0, 0, 0))

    def test_world_color_refracted_internal(self):
        w = DefaultWorld()
        shape = w.shapes[0]
        shape.material.transparency = 1.0
        shape.material.refractive_index = 1.5
        r = Ray(Point(0, 0, math.sqrt(2) / 2), Vector(0, 1, 0))
        xs = Intersections(
            [
                Intersection(-math.sqrt(2) / 2, shape),
                Intersection(math.sqrt(2) / 2, shape),
            ]
        )
        comps = xs[1].prepare_computations(r, xs)
        color = w.refracted_color(comps, 5)
        self.assertEqual(color, Color(0, 0, 0))

    def test_world_color_refracted_color(self):
        w = DefaultWorld()
        A = w.shapes[0]
        A.material.ambient = 1.0
        A.material.pattern = Pattern()
        B = w.shapes[1]
        B.material.transparency = 1.0
        B.material.refractive_index = 1.5
        r = Ray(Point(0, 0, 0.1), Vector(0, 1, 0))
        xs = Intersections(
            [
                Intersection(-0.9899, A),
                Intersection(-0.4899, B),
                Intersection(0.4899, B),
                Intersection(0.9899, A),
            ]
        )
        comps = xs[2].prepare_computations(r, xs)
        color = w.refracted_color(comps, 5)
        self.assertEqual(color, Color(0, 0.99888, 0.04725))

    def test_world_shade_hit_transparent(self):
        w = DefaultWorld()

        floor = Plane()
        floor.transform = Transform().translation(0, -1, 0)
        floor.material.transparency = 0.5
        floor.material.refractive_index = 1.5
        w.shapes.append(floor)

        ball = Sphere()
        ball.material.color = Color(1, 0, 0)
        ball.material.ambient = 0.5
        ball.transform = Transform().translation(0, -3.5, -0.5)
        w.shapes.append(ball)

        r = Ray(Point(0, 0, -3), Vector(0, -math.sqrt(2) / 2, math.sqrt(2) / 2))
        xs = Intersections([Intersection(math.sqrt(2), floor)])
        comps = xs[0].prepare_computations(r, xs)
        color = w.shade_hit(comps, 5)
        self.assertEqual(color, Color(0.93642, 0.68642, 0.68642))

    def test_world_shade_hit_reflective_transparent(self):
        w = DefaultWorld()

        floor = Plane()
        floor.transform = Transform().translation(0, -1, 0)
        floor.material.reflective = 0.5
        floor.material.transparency = 0.5
        floor.material.refractive_index = 1.5
        w.shapes.append(floor)

        ball = Sphere()
        ball.material.color = Color(1, 0, 0)
        ball.material.ambient = 0.5
        ball.transform = Transform().translation(0, -3.5, -0.5)
        w.shapes.append(ball)

        r = Ray(Point(0, 0, -3), Vector(0, -math.sqrt(2) / 2, math.sqrt(2) / 2))
        xs = Intersections([Intersection(math.sqrt(2), floor)])
        comps = xs[0].prepare_computations(r, xs)
        color = w.shade_hit(comps, 5)
        self.assertEqual(color, Color(0.93391, 0.69643, 0.69243))
