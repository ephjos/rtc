import math

from rtc.intersection import Intersection, Intersections
from rtc.materials import Material
from rtc.ray import Ray
from rtc.shape import Shape
from rtc.tuples import Tuple4, Point

from typing import Optional


class Sphere(Shape):
    def __init__(self, material: Optional[Material] = None):
        super().__init__(material)

    def local_intersect(self, ray: "Ray") -> "Intersections":
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

    def local_normal_at(self, point: Tuple4) -> Tuple4:
        return point - Point(0, 0, 0)


def GlassSphere() -> Sphere:
    return Sphere(Material(transparency=1.0, refractive_index=1.5))
