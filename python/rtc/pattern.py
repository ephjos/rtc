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
        return Color(point.x, point.y, point.z)

    def pattern_at_shape(self, shape: "Shape", point: "Tuple4") -> "Color":
        shape_point = shape.inverse_transform @ point
        pattern_point = self.inverse_transform @ shape_point
        return self.pattern_at(pattern_point)


class StripePattern(Pattern):
    def __init__(self, a: Color, b: Color):
        super().__init__()
        self.a = a
        self.b = b

    def pattern_at(self, point: "Tuple4") -> Color:
        x = 0 if abs(point.x) < EPSILON else math.floor(point.x)
        if x % 2 == 0:
            return self.a
        return self.b


class GradientPattern(Pattern):
    def __init__(self, a: Color, b: Color):
        super().__init__()
        self.a = a
        self.b = b

    def pattern_at(self, point: "Tuple4") -> Color:
        return self.a + (self.b - self.a) * (point.x - math.floor(point.x))


class RingPattern(Pattern):
    def __init__(self, a: Color, b: Color):
        super().__init__()
        self.a = a
        self.b = b

    def pattern_at(self, point: "Tuple4") -> Color:
        x2 = point.x * point.x
        z2 = point.z * point.z
        if math.floor(math.sqrt(x2 + z2)) % 2 == 0:
            return self.a
        return self.b


class CheckersPattern(Pattern):
    def __init__(self, a: Color, b: Color):
        super().__init__()
        self.a = a
        self.b = b

    def pattern_at(self, point: "Tuple4") -> Color:
        x = 0 if abs(point.x) < EPSILON else math.floor(point.x)
        y = 0 if abs(point.y) < EPSILON else math.floor(point.y)
        z = 0 if abs(point.z) < EPSILON else math.floor(point.z)
        if (x + y + z) % 2 == 0:
            return self.a
        return self.b
