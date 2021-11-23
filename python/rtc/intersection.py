import math

from rtc.ray import Ray
from rtc.shape import Shape
from rtc.tuples import *
from rtc.utils import EPSILON

from dataclasses import dataclass, field
from typing import Optional


@dataclass
class Computations:
    t: float
    shape: Shape
    point: Tuple4
    eyev: Tuple4
    normalv: Tuple4
    reflectv: Tuple4
    inside: bool
    over_point: Tuple4
    under_point: Tuple4
    n1: float
    n2: float

    def schlick(self):
        cos = tuple4_dot(self.eyev, self.normalv)

        if self.n1 > self.n2:
            n = self.n1 / self.n2
            sin2_t = (n * n) * (1.0 - (cos * cos))
            if sin2_t > 1.0:
                return 1.0

            cos_t = math.sqrt(1 - sin2_t)
            cos = cos_t

        r0 = ((self.n1 - self.n2) / (self.n1 + self.n2)) ** 2
        return r0 + (1 - r0) * (1 - cos) ** 5


@dataclass
class Intersection:
    t: float
    shape: Shape

    def prepare_computations(self, ray: Ray, xs: "Intersections") -> Computations:
        t = self.t
        shape = self.shape
        point = ray.position(self.t)
        eyev = tuple4_neg(ray.direction)
        normalv = self.shape.normal_at(point)
        reflectv = tuple4_reflect(ray.direction, normalv)
        inside = False

        if tuple4_dot(normalv, eyev) < 0:
            inside = True
            normalv = tuple4_neg(normalv)

        over_point = tuple4_add(point, tuple4_scale(normalv, EPSILON))
        under_point = tuple4_sub(point, tuple4_scale(normalv, EPSILON))

        containers: list[Shape] = []
        hit = self
        n1 = 0.0
        n2 = 0.0

        for i in xs:
            if i == hit:
                if len(containers) == 0:
                    n1 = 1.0
                else:
                    n1 = containers[-1].material.refractive_index
            if i.shape in containers:
                containers.remove(i.shape)
            else:
                containers.append(i.shape)
            if i == hit:
                if len(containers) == 0:
                    n2 = 1.0
                else:
                    n2 = containers[-1].material.refractive_index
                break

        return Computations(
            t,
            shape,
            point,
            eyev,
            normalv,
            reflectv,
            inside,
            over_point,
            under_point,
            n1,
            n2,
        )


@dataclass
class Intersections:
    data: list[Intersection] = field(default_factory=list)
    i: int = 0

    def __getitem__(self, index: int) -> Intersection:
        return self.data[index]

    def __len__(self):
        return len(self.data)

    def __iter__(self):
        return self.data.__iter__()

    def hit(self, shadow: bool = False) -> Optional[Intersection]:
        if len(self) == 0:
            return None

        positive_intersections = list(filter(lambda i: i.t >= 0, self.data))

        intersections = positive_intersections
        if shadow:
            intersections = list(filter(lambda i: i.shape.cast_shadow, intersections))

        if len(intersections) == 0:
            return None

        curr = intersections[0]
        for i in intersections[1:]:
            if i.t > 0 and i.t < curr.t:
                curr = i

        if curr.t < 0:
            return None
        return curr
