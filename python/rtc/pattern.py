import math

from rtc.color import Color
from rtc.transform import Transform
from rtc.utils import EPSILON


class Pattern:
    def __init__(self):
        self._transform = Transform()
        self.inverse_transform = self.transform.inverse()

    @property
    def transform(self):
        return self._transform

    @transform.setter
    def transform(self, t):
        self._transform = t
        self.inverse_transform = t.inverse()

    def pattern_at(self, point):
        return Color(point.x, point.y, point.z)

    def pattern_at_shape(self, o, point):
        object_point = o.inverse_transform @ point
        pattern_point = self.inverse_transform @ object_point
        return self.pattern_at(pattern_point)


class StripePattern(Pattern):
    def __init__(self, a, b):
        super().__init__()
        self.a = a
        self.b = b

    def pattern_at(self, point):
        if math.floor(point.x) % 2 == 0:
            return self.a
        return self.b


class GradientPattern(Pattern):
    def __init__(self, a, b):
        super().__init__()
        self.a = a
        self.b = b

    def pattern_at(self, point):
        return self.a + (self.b - self.a) * (point.x - math.floor(point.x))


class RingPattern(Pattern):
    def __init__(self, a, b):
        super().__init__()
        self.a = a
        self.b = b

    def pattern_at(self, point):
        x2 = point.x * point.x
        z2 = point.z * point.z
        if math.floor(math.sqrt(x2 + z2)) % 2 == 0:
            return self.a
        return self.b


class CheckersPattern(Pattern):
    def __init__(self, a, b):
        super().__init__()
        self.a = a
        self.b = b

    def pattern_at(self, point):
        x = 0 if abs(point.x) < EPSILON else math.floor(point.x)
        y = 0 if abs(point.y) < EPSILON else math.floor(point.y)
        z = 0 if abs(point.z) < EPSILON else math.floor(point.z)
        if (x + y + z) % 2 == 0:
            return self.a
        return self.b
