from rtc.intersection import Intersection, Intersections
from rtc.materials import Material
from rtc.ray import Ray
from rtc.shape import Shape
from rtc.tuples import Tuple4
from rtc.tuples import Vector
from rtc.utils import req

from typing import Optional


class Plane(Shape):
    def __init__(self, material: Optional[Material] = None):
        super().__init__(material)

    def local_intersect(self, ray: "Ray") -> Intersections:
        if req(ray.direction.y, 0):
            return Intersections()

        t = -ray.origin.y / ray.direction.y
        return Intersections([Intersection(t, self)])

    def local_normal_at(self, point: "Tuple4") -> "Tuple4":
        return Vector(0, 1, 0)
