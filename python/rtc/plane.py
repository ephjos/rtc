import math
import uuid

from rtc.intersection import Intersection, Intersections
from rtc.materials import Material
from rtc.shape import Shape
from rtc.transform import Transform
from rtc.tuples import Point, Vector
from rtc.utils import req


class Plane(Shape):
    def __init__(self, material=None):
        super().__init__(material)

    def local_intersect(self, ray):
        if req(ray.direction.y, 0):
            return Intersections()

        t = -ray.origin.y / ray.direction.y
        return Intersections([Intersection(t, self)])

    def local_normal_at(self, point):
        return Vector(0,1,0)

