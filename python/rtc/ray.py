from rtc.tuples import Tuple4
from rtc.transform import Transform

from typing import cast


class Ray:
    def __init__(self, origin: "Tuple4", direction: "Tuple4"):
        if not origin.isPoint():
            raise Exception("origin must be a point")
        if not direction.isVector():
            raise Exception("direction must be a vector")
        self.origin = origin
        self.direction = direction

    def position(self, t: float) -> "Tuple4":
        return self.origin + self.direction * t

    def transform(self, transform: "Transform") -> "Ray":
        origin = transform @ self.origin
        direction = transform @ self.direction

        if not (isinstance(origin, Tuple4) or isinstance(direction, Tuple4)):
            raise NotImplementedError()

        return Ray(cast(Tuple4, origin), cast(Tuple4, direction))
