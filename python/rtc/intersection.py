from rtc.ray import Ray
from rtc.shape import Shape
from rtc.tuples import Tuple4
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


@dataclass
class Intersection:
    t: float
    shape: Shape

    def prepare_computations(self, ray: Ray) -> Computations:
        t = self.t
        shape = self.shape
        point = ray.position(self.t)
        eyev = -ray.direction
        normalv = self.shape.normal_at(point)
        reflectv = ray.direction.reflect(normalv)
        inside = False

        if normalv.dot(eyev) < 0:
            inside = True
            normalv = -normalv

        over_point = point + normalv * EPSILON

        return Computations(
            t, shape, point, eyev, normalv, reflectv, inside, over_point
        )


@dataclass
class Intersections:
    data: list[Intersection] = field(default_factory=list)

    def __getitem__(self, index: int) -> Intersection:
        return self.data[index]

    def __len__(self):
        return len(self.data)

    def __iter__(self):
        return self.data.__iter__()

    def hit(self) -> Optional[Intersection]:
        if len(self) == 0:
            return None

        positive_intersections = list(filter(lambda i: i.t >= 0, self.data))
        if len(positive_intersections) == 0:
            return None

        curr = positive_intersections[0]
        for i in positive_intersections[1:]:
            if i.t > 0 and i.t < curr.t:
                curr = i

        if curr.t < 0:
            return None
        return curr
