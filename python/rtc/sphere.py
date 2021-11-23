import math

from rtc.intersection import Intersection, Intersections
from rtc.materials import Material
from rtc.ray import Ray
from rtc.shape import Shape
from rtc.tuples import *

from typing import Optional


class Sphere(Shape):
    def __init__(self, material: Optional[Material] = None):
        super().__init__(material)

    def local_intersect(self, ray: "Ray") -> "Intersections":
        sphere_to_ray = tuple4_sub(ray.origin, Point(0, 0, 0))
        a = tuple4_dot(ray.direction, ray.direction)
        b = tuple4_dot(ray.direction, sphere_to_ray) * 2
        c = tuple4_dot(sphere_to_ray, sphere_to_ray) - 1
        discriminant = (b * b) - 4 * a * c

        if discriminant < 0:
            return Intersections([])

        t1 = (-b - math.sqrt(discriminant)) / (2 * a)
        t2 = (-b + math.sqrt(discriminant)) / (2 * a)

        if t1 > t2:
            return Intersections([Intersection(t2, self), Intersection(t1, self)])
        return Intersections([Intersection(t1, self), Intersection(t2, self)])

    def local_normal_at(self, point: Tuple4) -> Tuple4:
        return tuple4_sub(point, Point(0, 0, 0))


def GlassSphere() -> Sphere:
    return Sphere(Material(transparency=1.0, refractive_index=1.5))
