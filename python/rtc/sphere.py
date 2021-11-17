import math

from rtc.intersection import Intersection, Intersections
from rtc.shape import Shape
from rtc.tuples import Point


class Sphere(Shape):
    def __init__(self, material=None):
        super().__init__(material)

    def local_intersect(self, ray):
        sphere_to_ray = ray.origin - Point(0, 0, 0)
        a = ray.direction.dot(ray.direction)
        b = 2 * ray.direction.dot(sphere_to_ray)
        c = sphere_to_ray.dot(sphere_to_ray) - 1
        discriminant = (b * b) - 4 * a * c

        if discriminant < 0:
            return Intersections()

        t1 = (-b - math.sqrt(discriminant)) / (2 * a)
        t2 = (-b + math.sqrt(discriminant)) / (2 * a)

        if t1 > t2:
            return Intersections([Intersection(t2, self), Intersection(t1, self)])
        return Intersections([Intersection(t1, self), Intersection(t2, self)])

    def local_normal_at(self, point):
        return point - Point(0, 0, 0)
