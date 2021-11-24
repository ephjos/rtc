import math

from rtc.intersection import Intersection, Intersections
from rtc.materials import Material
from rtc.ray import Ray
from rtc.shape import Shape
from rtc.tuples import *
from rtc.utils import *

from typing import Optional


class Cone(Shape):
    def __init__(self, material: Optional[Material] = None):
        super().__init__(material)
        self.minimum = float("-inf")
        self.maximum = float("inf")
        self.closed = False

    def check_cap(self, ray, t, y):
        x = ray.origin[0] + t * ray.direction[0]
        z = ray.origin[2] + t * ray.direction[2]
        return (x**2 + z**2) <= abs(y)

    def intersect_caps(self, ray):
        if not self.closed or req(ray.direction[1], 0):
            return []

        xs = []
        t = (self.minimum - ray.origin[1]) / ray.direction[1]
        if self.check_cap(ray, t, self.minimum):
            xs.append(Intersection(t, self))

        t = (self.maximum - ray.origin[1]) / ray.direction[1]
        if self.check_cap(ray, t, self.maximum):
            xs.append(Intersection(t, self))

        return xs


    def local_intersect(self, ray: "Ray") -> "Intersections":
        a = ray.direction[0]**2 - ray.direction[1]**2 + ray.direction[2]**2
        b = (2 * ray.origin[0] * ray.direction[0]) - \
            (2 * ray.origin[1] * ray.direction[1]) + \
            (2 * ray.origin[2] * ray.direction[2])
        c = ray.origin[0]**2 - ray.origin[1]**2 + ray.origin[2]**2

        if req(a, 0) and req(b, 0):
            return self.intersect_caps(ray)
        elif req(a, 0):
            t = -c / (2*b)
            xs = [Intersection(t, self)]
            xs += self.intersect_caps(ray)
            return Intersections(xs)

        disc = b**2 - 4*a*c

        if disc < 0:
            return Intersections([])

        t0 = (-b - math.sqrt(disc)) / (2*a)
        t1 = (-b + math.sqrt(disc)) / (2*a)

        if t0 > t1:
            t0,t1 = t1,t0

        xs = []

        y0 = ray.origin[1] + t0*ray.direction[1]
        if self.minimum < y0 and y0 < self.maximum:
            xs.append(Intersection(t0, self))

        y1 = ray.origin[1] + t1*ray.direction[1]
        if self.minimum < y1 and y1 < self.maximum:
            xs.append(Intersection(t1, self))

        xs += self.intersect_caps(ray)
        return Intersections(xs)

    def local_normal_at(self, point: Tuple4) -> Tuple4:
        dist = point[0]**2 + point[2]**2

        if dist < 1 and point[1] >= self.maximum-EPSILON:
            return Vector(0,1,0)

        if dist < 1 and point[1] <= self.minimum+EPSILON:
            return Vector(0,-1,0)

        y = math.sqrt(point[0]**2 + point[2]**2)
        if point[1] > 0:
            y *= -1

        return Vector(point[0], y, point[2])

