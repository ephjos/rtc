import math

from rtc.intersection import Intersection, Intersections
from rtc.materials import Material
from rtc.ray import Ray
from rtc.shape import Shape
from rtc.tuples import Tuple4, Point, Vector
from rtc.utils import EPSILON

from typing import Optional


class Cube(Shape):
    def __init__(self, material: Optional[Material] = None):
        super().__init__(material)

    def check_axis(self, origin, direction):
        tmin_numerator = -1 - origin
        tmax_numerator = 1 - origin

        if abs(direction) >= EPSILON:
            tmin = tmin_numerator / direction
            tmax = tmax_numerator / direction
        else:
            tmin = tmin_numerator * float('inf')
            tmax = tmax_numerator * float('inf')

        if tmin > tmax:
            tmin, tmax = tmax, tmin

        return tmin, tmax

    def local_intersect(self, ray: "Ray") -> "Intersections":
        xtmin, xtmax = self.check_axis(ray.origin.x, ray.direction.x)
        ytmin, ytmax = self.check_axis(ray.origin.y, ray.direction.y)
        ztmin, ztmax = self.check_axis(ray.origin.z, ray.direction.z)

        tmin = max(xtmin, ytmin, ztmin)
        tmax = min(xtmax, ytmax, ztmax)

        if tmin > tmax:
            return Intersections([])

        return Intersections([Intersection(tmin, self), Intersection(tmax, self)])

    def local_normal_at(self, point: Tuple4) -> Tuple4:
        maxc = max(abs(point.x), abs(point.y), abs(point.z))

        if maxc == abs(point.x):
            return Vector(point.x, 0, 0)
        elif maxc == abs(point.y):
            return Vector(0, point.y, 0)

        return Vector(0, 0, point.z)

