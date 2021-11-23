import math

from rtc.color import Color
from rtc.transform import Transform
from rtc.tuples import Tuple4
from rtc.utils import EPSILON

from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from rtc.shape import Shape


class Pattern:
    def __init__(self):
        self._transform = Transform()
        self.inverse_transform = self.transform.inverse()

    @property
    def transform(self) -> Transform:
        return self._transform

    @transform.setter
    def transform(self, transform: Transform):
        self._transform = transform
        self.inverse_transform = transform.inverse()

    def pattern_at(self, point: "Tuple4") -> Color:
        return Color(point[0], point[1], point[2])

    def pattern_at_shape(self, shape: "Shape", point: "Tuple4") -> "Color":
        shape_point = shape.inverse_transform.matmul_tuple(point)
        pattern_point = self.inverse_transform.matmul_tuple(shape_point)
        return self.pattern_at(pattern_point)


class StripePattern(Pattern):
    def __init__(self, a: Color, b: Color):
        super().__init__()
        self.a = a
        self.b = b

    def pattern_at(self, point: "Tuple4") -> Color:
        x = 0 if abs(point[0]) < EPSILON else math.floor(point[0])
        if x % 2 == 0:
            return self.a
        return self.b


class GradientPattern(Pattern):
    def __init__(self, a: Color, b: Color):
        super().__init__()
        self.a = a
        self.b = b

    def pattern_at(self, point: "Tuple4") -> Color:
        return self.a + (self.b - self.a) * (point[0] - math.floor(point[0]))


class RingPattern(Pattern):
    def __init__(self, a: Color, b: Color):
        super().__init__()
        self.a = a
        self.b = b

    def pattern_at(self, point: "Tuple4") -> Color:
        x2 = point[0] * point[0]
        z2 = point[2] * point[2]
        if math.floor(math.sqrt(x2 + z2)) % 2 == 0:
            return self.a
        return self.b


class CheckersPattern(Pattern):
    def __init__(self, a: Color, b: Color):
        super().__init__()
        self.a = a
        self.b = b

    def pattern_at(self, point: "Tuple4") -> Color:
        x = 0 if abs(point[0]) < EPSILON else math.floor(point[0])
        y = 0 if abs(point[1]) < EPSILON else math.floor(point[1])
        z = 0 if abs(point[2]) < EPSILON else math.floor(point[2])
        if (x + y + z) % 2 == 0:
            return self.a
        return self.b
