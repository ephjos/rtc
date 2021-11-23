from rtc.tuples import *
from rtc.transform import Transform


class Ray:
    def __init__(self, origin: "Tuple4", direction: "Tuple4"):
        if not tuple4_is_point(origin):
            raise Exception("origin must be a point")
        if not tuple4_is_vector(direction):
            raise Exception("direction must be a vector")
        self.origin = origin
        self.direction = direction

    def position(self, t: float) -> "Tuple4":
        return tuple4_add(self.origin, tuple4_scale(self.direction, t))

    def transform(self, transform: "Transform") -> "Ray":
        origin = transform @ self.origin
        direction = transform @ self.direction

        return Ray(origin, direction)
