#!/usr/bin/env python

import math
import random
import unittest
import uuid

from canvas import Canvas
from color import Color
from intersection import Intersection, Intersections
from lights import PointLight
from materials import Material
from ray import Ray
from transform import Transform
from tuples import Tuple, Point, Vector

class Sphere:
    def __init__(self, material=Material()):
        self.id = str(uuid.uuid4())

        self._transform = Transform()
        self.inverse_transform = self.transform.inverse()

        self.material = material

    @property
    def transform(self):
        return self._transform

    @transform.setter
    def transform(self, t):
        self._transform = t
        self.inverse_transform = t.inverse()

    def intersect(self, r):
        ray = r.transform(self.inverse_transform)
        sphere_to_ray = ray.origin - Point(0,0,0)
        a = ray.direction.dot(ray.direction)
        b = 2 * ray.direction.dot(sphere_to_ray)
        c = sphere_to_ray.dot(sphere_to_ray) - 1
        discriminant = (b*b) - 4*a*c

        if discriminant < 0:
            return Intersections()

        t1 = (-b - math.sqrt(discriminant)) / (2*a)
        t2 = (-b + math.sqrt(discriminant)) / (2*a)

        if t1 > t2:
            return Intersections(Intersection(t2,self), Intersection(t1, self))
        return Intersections(Intersection(t1,self), Intersection(t2, self))

    def normal_at(self, world_point):
        object_point = self.inverse_transform @ world_point
        object_normal = object_point - Point(0,0,0)
        world_normal = self.inverse_transform.T @ object_normal
        world_normal.w = 0
        return world_normal.normalize()

class TestSphere(unittest.TestCase):
    def test_sphere_intersect(self):
        r = Ray(Point(0,0,-5), Vector(0,0,1))
        s = Sphere()
        xs = s.intersect(r)

        self.assertEqual(len(xs), 2)
        self.assertEqual(xs[0].t, 4.0)
        self.assertEqual(xs[1].t, 6.0)

    def test_sphere_intersect_tangent(self):
        r = Ray(Point(0,1,-5), Vector(0,0,1))
        s = Sphere()
        xs = s.intersect(r)

        self.assertEqual(len(xs), 2)
        self.assertEqual(xs[0].t, 5.0)
        self.assertEqual(xs[1].t, 5.0)

    def test_sphere_intersect_miss(self):
        r = Ray(Point(0,2,-5), Vector(0,0,1))
        s = Sphere()
        xs = s.intersect(r)

        self.assertEqual(len(xs), 0)

    def test_sphere_intersect_inside(self):
        r = Ray(Point(0,0,0), Vector(0,0,1))
        s = Sphere()
        xs = s.intersect(r)

        self.assertEqual(len(xs), 2)
        self.assertEqual(xs[0].t, -1.0)
        self.assertEqual(xs[1].t, 1.0)

    def test_sphere_intersect_behind(self):
        r = Ray(Point(0,0,5), Vector(0,0,1))
        s = Sphere()
        xs = s.intersect(r)

        self.assertEqual(len(xs), 2)
        self.assertEqual(xs[0].t, -6.0)
        self.assertEqual(xs[1].t, -4.0)

    def test_sphere_intersect_set_object(self):
        r = Ray(Point(0,0,-5), Vector(0,0,1))
        s = Sphere()
        xs = s.intersect(r)

        self.assertEqual(len(xs), 2)
        self.assertEqual(xs[0].object, s)
        self.assertEqual(xs[1].object, s)

    def test_intersection_constructor(self):
        s = Sphere()
        i = Intersection(3.5, s)

        self.assertEqual(i.t, 3.5)
        self.assertEqual(i.object, s)

    def test_intersections_constructor(self):
        s = Sphere()
        i1 = Intersection(1, s)
        i2 = Intersection(2, s)
        xs = Intersections(i1, i2)

        self.assertEqual(len(xs), 2)
        self.assertEqual(xs[0].t, 1)
        self.assertEqual(xs[1].t, 2)

    def test_intersections_positive_t(self):
        s = Sphere()
        i1 = Intersection(1, s)
        i2 = Intersection(2, s)
        xs = Intersections(i2, i1)
        i = xs.hit()

        self.assertEqual(i, i1)

    def test_intersections_negative_t(self):
        s = Sphere()
        i1 = Intersection(-1, s)
        i2 = Intersection(1, s)
        xs = Intersections(i2, i1)
        i = xs.hit()

        self.assertEqual(i, i2)

    def test_intersections_all_negative_t(self):
        s = Sphere()
        i1 = Intersection(-2, s)
        i2 = Intersection(-1, s)
        xs = Intersections(i2, i1)
        i = xs.hit()

        self.assertEqual(i, None)

    def test_intersections_lowest_non_negative(self):
        s = Sphere()
        i1 = Intersection(5, s)
        i2 = Intersection(7, s)
        i3 = Intersection(-3, s)
        i4 = Intersection(2, s)
        xs = Intersections(i1, i2, i3, i4)
        i = xs.hit()

        self.assertEqual(i, i4)

    def test_sphere_default_transform(self):
        s = Sphere()
        self.assertEqual(s.transform, Transform())

    def test_sphere_change_transform(self):
        s = Sphere()
        t = Transform().translation(2,3,4)
        s.transform = t
        self.assertEqual(s.transform, t)

    def test_sphere_intersect_scaled(self):
        r = Ray(Point(0,0,-5), Vector(0,0,1))
        s = Sphere()
        s.transform = Transform().scaling(2,2,2)
        xs = s.intersect(r)

        self.assertEqual(len(xs), 2)
        self.assertEqual(xs[0].t, 3)
        self.assertEqual(xs[1].t, 7)

    def test_sphere_intersect_translated(self):
        r = Ray(Point(0,0,-5), Vector(0,0,1))
        s = Sphere()
        s.transform = Transform().translation(5,0,0)
        xs = s.intersect(r)

        self.assertEqual(len(xs), 0)

    def test_sphere_normal_at_x(self):
        s = Sphere()
        n = s.normal_at(Point(1, 0, 0))
        self.assertEqual(n, Vector(1,0,0))

    def test_sphere_normal_at_y(self):
        s = Sphere()
        n = s.normal_at(Point(0, 1, 0))
        self.assertEqual(n, Vector(0,1,0))

    def test_sphere_normal_at_z(self):
        s = Sphere()
        n = s.normal_at(Point(0, 0, 1))
        self.assertEqual(n, Vector(0,0,1))

    def test_sphere_normal_at_non_axis(self):
        s = Sphere()
        r3o3 = math.sqrt(3)/3
        n = s.normal_at(Point(r3o3, r3o3, r3o3))
        self.assertEqual(n, Vector(r3o3, r3o3, r3o3))
        self.assertEqual(n, n.normalize())

    def test_sphere_normal_on_translated(self):
        s = Sphere()
        s.transform = Transform().translation(0,1,0)
        n = s.normal_at(Point(0, 1.70711, -0.70711))
        self.assertEqual(n, Vector(0, 0.70711, -0.70711))

    def test_sphere_normal_on_transformed(self):
        s = Sphere()
        s.transform = Transform().rotation_z(math.pi/5).scaling(1, 0.5, 1)
        n = s.normal_at(Point(0, math.sqrt(2)/2, -math.sqrt(2)/2))
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

def demo_ray(*args):
    origin = Point(0,0,-5)
    wall_z = 10
    wall_size = 7
    s = Sphere()

    hit_color = Color(1,0,0)

    canvas_size = 100
    pixel_size = wall_size / canvas_size
    c = Canvas(canvas_size, canvas_size)
    w2 = c.w // 2
    h2 = c.h // 2

    for i in range(-h2,h2):
        world_y = i * pixel_size
        for j in range(-w2,w2):
            world_x = j * pixel_size
            ray = Ray(origin, (Point(world_x,world_y,wall_z)-origin).normalize())
            xs = s.intersect(ray)
            hit = xs.hit()

            if hit is None:
                continue
            c.write(i+h2, j+w2, hit_color)

    c.save("./demo_ray.ppm")


def demo_material(*args):
    origin = Point(0,0,-5)
    wall_z = 10
    wall_size = 7
    s = Sphere()
    s.material.color = Color(1,0.2,1)

    light_position = Point(-10, -10, -10)
    light_color = Color(1,1,1)
    light = PointLight(light_position, light_color)

    canvas_size = 480
    pixel_size = wall_size / canvas_size
    c = Canvas(canvas_size, canvas_size)
    w2 = c.w // 2
    h2 = c.h // 2

    for i in range(-h2,h2):
        world_y = i*pixel_size
        for j in range(-w2,w2):
            world_x = j*pixel_size
            ray = Ray(origin, (Point(world_x,world_y,wall_z)-origin).normalize())
            xs = s.intersect(ray)
            hit = xs.hit()

            if hit is None:
                continue

            point = ray.position(hit.t)
            normal = hit.object.normal_at(point)
            eye = -ray.direction
            color = s.material.lighting(light,point,eye,normal)
            c.write(j+w2, (i+h2), color)

    c.save("./demo_material.ppm")
