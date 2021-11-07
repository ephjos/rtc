
import unittest

from rtc.color import Color
from rtc.intersection import Intersection
from rtc.lights import PointLight
from rtc.ray import Ray
from rtc.sphere import Sphere
from rtc.transform import Transform
from rtc.tuples import Point, Vector
from rtc.world import World, DefaultWorld


class TestWorld(unittest.TestCase):
    def test_world_constructor(self):
        w = World()
        self.assertEqual(w.objects, [])
        self.assertEqual(w.light, None)

    def test_default_world(self):
        light = PointLight(Point(-10,10,-10), Color(1,1,1))
        s1 = Sphere()
        s1.material.color = Color(0.8, 1.0, 0.6)
        s1.material.diffuse = 0.7
        s1.material.specular = 0.2

        s2 = Sphere()
        s2.transform = Transform().scaling(0.5, 0.5, 0.5)

        w = DefaultWorld()

        self.assertEqual(w.light, light)
        self.assertTrue(s1 in w.objects)
        self.assertTrue(s2 in w.objects)

    def test_world_intersect(self):
        w = DefaultWorld()
        r = Ray(Point(0,0,-5), Vector(0,0,1))

        xs = w.intersect(r)

        self.assertEqual(len(xs), 4)
        self.assertEqual(xs[0].t, 4)
        self.assertEqual(xs[1].t, 4.5)
        self.assertEqual(xs[2].t, 5.5)
        self.assertEqual(xs[3].t, 6)

    def test_world_shade_hit(self):
        w = DefaultWorld()
        r = Ray(Point(0,0,-5), Vector(0,0,1))
        shape = w.objects[0]
        i = Intersection(4, shape)
        comps = i.prepare_computations(r)
        c = w.shade_hit(comps)

        self.assertEqual(c, Color(0.38066, 0.47583, 0.2855))

    def test_world_shade_hit_inside(self):
        w = DefaultWorld()
        w.light = PointLight(Point(0,0.25,0), Color(1,1,1))
        r = Ray(Point(0,0,0), Vector(0,0,1))
        shape = w.objects[1]
        i = Intersection(0.5, shape)

        comps = i.prepare_computations(r)
        c = w.shade_hit(comps)

        self.assertEqual(c, Color(0.90498, 0.90498, 0.90498))

    def test_color_at_misses(self):
        w = DefaultWorld()
        r = Ray(Point(0,0,-5), Vector(0,1,0))
        c = w.color_at(r)

        self.assertEqual(c, Color(0,0,0))

    def test_color_at_hits(self):
        w = DefaultWorld()
        r = Ray(Point(0,0,-5), Vector(0,0,1))
        c = w.color_at(r)

        self.assertEqual(c, Color(0.38066, 0.47583, 0.2855))

    def test_color_at_hits_behind(self):
        w = DefaultWorld()
        outer = w.objects[0]
        outer.material.ambient = 1
        inner = w.objects[1]
        inner.material.ambient = 1

        r = Ray(Point(0,0,0.75), Vector(0,0,-1))
        c = w.color_at(r)

        self.assertEqual(c, inner.material.color)


