#!/usr/bin/env python

import math
import random
import unittest
import uuid

from canvas import Canvas
from color import Color
from intersection import Intersection, Intersections
from ray import Ray
from transform import Transform
from tuples import Tuple, Point, Vector

class Sphere:
    def __init__(self):
        self.id = str(uuid.uuid4())
        self._transform = Transform.identity()
        self.inverse = self.transform.inverse()

    @property
    def transform(self):
        return self._transform

    @transform.setter
    def transform(self, t):
        self._transform = t
        self.inverse = t.inverse()

    def intersect(self, r):
        ray = r.transform(self.inverse)
        sphere_to_ray = ray.origin - Point(0,0,0)
        a = Tuple.dot(ray.direction, ray.direction)
        b = 2 * Tuple.dot(ray.direction, sphere_to_ray)
        c = Tuple.dot(sphere_to_ray, sphere_to_ray) - 1
        discriminant = (b*b) - 4*a*c

        if discriminant < 0:
            return Intersections()

        t1 = (-b - math.sqrt(discriminant)) / (2*a)
        t2 = (-b + math.sqrt(discriminant)) / (2*a)

        if t1 > t2:
            return Intersections(Intersection(t2,self), Intersection(t1, self))
        return Intersections(Intersection(t1,self), Intersection(t2, self))

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
        self.assertEqual(s.transform, Transform.identity())

    def test_sphere_change_transform(self):
        s = Sphere()
        t = Transform.identity().translation(2,3,4)
        s.transform = t
        self.assertEqual(s.transform, t)

    def test_sphere_intersect_scaled(self):
        r = Ray(Point(0,0,-5), Vector(0,0,1))
        s = Sphere()
        s.transform = Transform.identity().scaling(2,2,2)
        xs = s.intersect(r)

        self.assertEqual(len(xs), 2)
        self.assertEqual(xs[0].t, 3)
        self.assertEqual(xs[1].t, 7)

    def test_sphere_intersect_translated(self):
        r = Ray(Point(0,0,-5), Vector(0,0,1))
        s = Sphere()
        s.transform = Transform.identity().translation(5,0,0)
        xs = s.intersect(r)

        self.assertEqual(len(xs), 0)

def demo_ray(*args):
    origin = Point(0,0,-1.05)
    wall_z = 10
    s = Sphere()

    hit_color = Color(1,0,0)
    c = Canvas(100,100)
    w2 = c.w // 2
    h2 = c.h // 2

    for i in range(-h2,h2):
        for j in range(-w2,w2):
            ray = Ray(origin, (Point(i,j,wall_z)-origin).normalize())
            xs = s.intersect(ray)
            hit = xs.hit()

            if hit is None:
                continue
            c.write(i+h2, j+w2, hit_color)

    c.save("./demo_ray.ppm")

def demo_ray_book(*args):
    origin = Point(0,0,-5)
    wall_z = 10
    wall_size = 7
    canvas_size = 100
    pixel_size = wall_size / canvas_size
    half = wall_size/2

    canvas = Canvas(canvas_size, canvas_size)
    color = Color(1,0,0)
    s = Sphere()

    for y in range(canvas_size):
        world_y = half - pixel_size*y
        for x in range(canvas_size):
            world_x = - half + pixel_size*x
            position = Point(world_x, world_y, wall_z)
            r = Ray(origin, (position-origin).normalize())
            xs = s.intersect(r)

            if xs.hit() is not None:
                canvas.write(x,y,color)

    canvas.save("./demo_ray_book.ppm")


