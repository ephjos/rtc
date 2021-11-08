import math
import uuid

from rtc.intersection import Intersection, Intersections
from rtc.materials import Material
from rtc.transform import Transform
from rtc.tuples import Point


class Sphere:
    def __init__(self, material=None):
        if material is None:
            material = Material()
        self.id = str(uuid.uuid4())

        self._transform = Transform()
        self.inverse_transform = self.transform.inverse()

        self.material = material

    def __eq__(self, o):
        return self.transform == o.transform and self.material == o.material

    @property
    def transform(self):
        return self._transform

    @transform.setter
    def transform(self, t):
        self._transform = t
        self.inverse_transform = t.inverse()

    def intersect(self, r):
        ray = r.transform(self.inverse_transform)
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

    def normal_at(self, world_point):
        object_point = self.inverse_transform @ world_point
        object_normal = object_point - Point(0, 0, 0)
        world_normal = self.inverse_transform.T @ object_normal
        world_normal.w = 0
        return world_normal.normalize()
