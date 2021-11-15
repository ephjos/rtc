import math
import unittest

from rtc.intersection import Intersection, Intersections
from rtc.materials import Material
from rtc.ray import Ray
from rtc.shape import Shape
from rtc.sphere import Sphere
from rtc.transform import Transform
from rtc.tuples import Point, Vector


class TestShape(unittest.TestCase):
    def test_shape_default_transformation(self):
        s = Shape()
        self.assertEqual(s.transform, Transform())

    def test_shape_assign_transformation(self):
        s = Shape()
        s.transform = Transform().translation(2,3,4)
        self.assertEqual(s.transform, Transform().translation(2,3,4))
        self.assertEqual(s.inverse_transform, Transform().translation(2,3,4).inverse())

    def test_shape_default_material(self):
        s = Shape()
        self.assertEqual(s.material, Material())

    def test_shape_assign_material(self):
        s = Shape()
        m = Material()
        m.ambient = 1
        s.material = m
        self.assertEqual(s.material, m)

    def test_shape_intersecting_scaled(self):
        r = Ray(Point(0,0,-5), Vector(0,0,1))
        s = Shape()
        s.transform = Transform().scaling(2,2,2)
        xs = s.intersect(r)
        self.assertEqual(s.saved_ray.origin, Point(0,0,-2.5))
        self.assertEqual(s.saved_ray.direction, Vector(0,0,0.5))

    def test_shape_intersecting_translated(self):
        r = Ray(Point(0,0,-5), Vector(0,0,1))
        s = Shape()
        s.transform = Transform().translation(5,0,0)
        xs = s.intersect(r)
        self.assertEqual(s.saved_ray.origin, Point(-5,0,-5))
        self.assertEqual(s.saved_ray.direction, Vector(0,0,1))

    def test_shape_normal_translated(self):
        s = Shape()
        s.transform = Transform().translation(0,1,0)
        n = s.normal_at(Point(0,1.70711,-0.70711))
        self.assertEqual(n, Vector(0,0.70711,-0.70711))

    def test_shape_normal_transformed(self):
        s = Shape()
        s.transform = Transform().rotation_z(math.pi/5).scaling(1,0.5,1)
        n = s.normal_at(Point(0,math.sqrt(2)/2,-math.sqrt(2)/2))
        self.assertEqual(n, Vector(0,0.97014,-0.24254))
