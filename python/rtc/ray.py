from rtc.tuples import *
from rtc.matrix import *


class Ray:
    def __init__(self, origin: "Tuple4", direction: "Tuple4"):
        self.origin = origin
        self.direction = direction

    def position(self, t: float) -> "Tuple4":
        return tuple4_add(self.origin, tuple4_scale(self.direction, t))

    def transform(self, transform: "Matrix", out: "Ray") -> "Ray":
        out.origin = matrix_mul_tuple(transform, self.origin)
        out.direction = matrix_mul_tuple(transform, self.direction)
