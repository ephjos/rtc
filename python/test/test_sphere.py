import math
import unittest

from rtc.intersection import Intersection, Intersections
from rtc.materials import Material
from rtc.ray import Ray
from rtc.sphere import Sphere, GlassSphere
from rtc.transform import Transform
from rtc.tuples import Point, Vector


class TestSphere(unittest.TestCase):
    def test_sphere_intersect(self):
        r = Ray(Point(0, 0, -5), Vector(0, 0, 1))
        s = Sphere()
        xs = s.intersect(r)

        self.assertEqual(len(xs), 2)
        self.assertEqual(xs[0].t, 4.0)
        self.assertEqual(xs[1].t, 6.0)

    def test_sphere_intersect_tangent(self):
        r = Ray(Point(0, 1, -5), Vector(0, 0, 1))
        s = Sphere()
        xs = s.intersect(r)

        self.assertEqual(len(xs), 2)
        self.assertEqual(xs[0].t, 5.0)
        self.assertEqual(xs[1].t, 5.0)

    def test_sphere_intersect_miss(self):
        r = Ray(Point(0, 2, -5), Vector(0, 0, 1))
        s = Sphere()
        xs = s.intersect(r)

        self.assertEqual(len(xs), 0)

    def test_sphere_intersect_inside(self):
        r = Ray(Point(0, 0, 0), Vector(0, 0, 1))
        s = Sphere()
        xs = s.intersect(r)

        self.assertEqual(len(xs), 2)
        self.assertEqual(xs[0].t, -1.0)
        self.assertEqual(xs[1].t, 1.0)

    def test_sphere_intersect_behind(self):
        r = Ray(Point(0, 0, 5), Vector(0, 0, 1))
        s = Sphere()
        xs = s.intersect(r)

        self.assertEqual(len(xs), 2)
        self.assertEqual(xs[0].t, -6.0)
        self.assertEqual(xs[1].t, -4.0)

    def test_sphere_intersect_set_shape(self):
        r = Ray(Point(0, 0, -5), Vector(0, 0, 1))
        s = Sphere()
        xs = s.intersect(r)

        self.assertEqual(len(xs), 2)
        self.assertEqual(xs[0].shape, s)
        self.assertEqual(xs[1].shape, s)

    def test_intersection_constructor(self):
        s = Sphere()
        i = Intersection(3.5, s)

        self.assertEqual(i.t, 3.5)
        self.assertEqual(i.shape, s)

    def test_intersections_constructor(self):
        s = Sphere()
        i1 = Intersection(1, s)
        i2 = Intersection(2, s)
        xs = Intersections([i1, i2])

        self.assertEqual(len(xs), 2)
        self.assertEqual(xs[0].t, 1)
        self.assertEqual(xs[1].t, 2)

    def test_intersections_positive_t(self):
        s = Sphere()
        i1 = Intersection(1, s)
        i2 = Intersection(2, s)
        xs = Intersections([i2, i1])
        i = xs.hit()

        self.assertEqual(i, i1)

    def test_intersections_negative_t(self):
        s = Sphere()
        i1 = Intersection(-1, s)
        i2 = Intersection(1, s)
        xs = Intersections([i2, i1])
        i = xs.hit()

        self.assertEqual(i, i2)

    def test_intersections_all_negative_t(self):
        s = Sphere()
        i1 = Intersection(-2, s)
        i2 = Intersection(-1, s)
        xs = Intersections([i2, i1])
        i = xs.hit()

        self.assertEqual(i, None)

    def test_intersections_lowest_non_negative(self):
        s = Sphere()
        i1 = Intersection(5, s)
        i2 = Intersection(7, s)
        i3 = Intersection(-3, s)
        i4 = Intersection(2, s)
        xs = Intersections([i1, i2, i3, i4])
        i = xs.hit()

        self.assertEqual(i, i4)

    def test_sphere_default_transform(self):
        s = Sphere()
        self.assertEqual(s.transform, Transform())

    def test_sphere_change_transform(self):
        s = Sphere()
        t = Transform().translation(2, 3, 4)
        s.transform = t
        self.assertEqual(s.transform, t)

    def test_sphere_intersect_scaled(self):
        r = Ray(Point(0, 0, -5), Vector(0, 0, 1))
        s = Sphere()
        s.transform = Transform().scaling(2, 2, 2)
        xs = s.intersect(r)

        self.assertEqual(len(xs), 2)
        self.assertEqual(xs[0].t, 3)
        self.assertEqual(xs[1].t, 7)

    def test_sphere_intersect_translated(self):
        r = Ray(Point(0, 0, -5), Vector(0, 0, 1))
        s = Sphere()
        s.transform = Transform().translation(5, 0, 0)
        xs = s.intersect(r)

        self.assertEqual(len(xs), 0)

    def test_sphere_normal_at_x(self):
        s = Sphere()
        n = s.normal_at(Point(1, 0, 0))
        self.assertEqual(n, Vector(1, 0, 0))

    def test_sphere_normal_at_y(self):
        s = Sphere()
        n = s.normal_at(Point(0, 1, 0))
        self.assertEqual(n, Vector(0, 1, 0))

    def test_sphere_normal_at_z(self):
        s = Sphere()
        n = s.normal_at(Point(0, 0, 1))
        self.assertEqual(n, Vector(0, 0, 1))

    def test_sphere_normal_at_non_axis(self):
        s = Sphere()
        r3o3 = math.sqrt(3) / 3
        n = s.normal_at(Point(r3o3, r3o3, r3o3))
        self.assertEqual(n, Vector(r3o3, r3o3, r3o3))
        self.assertEqual(n, n.normalize())

    def test_sphere_normal_on_translated(self):
        s = Sphere()
        s.transform = Transform().translation(0, 1, 0)
        n = s.normal_at(Point(0, 1.70711, -0.70711))
        self.assertEqual(n, Vector(0, 0.70711, -0.70711))

    def test_sphere_normal_on_transformed(self):
        s = Sphere()
        s.transform = Transform().rotation_z(math.pi / 5).scaling(1, 0.5, 1)
        n = s.normal_at(Point(0, math.sqrt(2) / 2, -math.sqrt(2) / 2))
        self.assertEqual(n, Vector(0, 0.97014, -0.24254))

    def test_sphere_default_material(self):
        s = Sphere()
        m = s.material
        self.assertEqual(m, Material())

    def test_sphere_assigned_material(self):
        s = Sphere()
        m = Material()
        m.ambient = 1
        s.material = m
        self.assertEqual(s.material, m)


class TestGlassSphere(unittest.TestCase):
    def test_glass_sphere(self):
        s = GlassSphere()
        self.assertEqual(s.transform, Transform())
        self.assertEqual(s.material.transparency, 1.0)
        self.assertEqual(s.material.refractive_index, 1.5)
