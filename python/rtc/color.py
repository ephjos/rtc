from rtc.utils import req, clamp

from dataclasses import dataclass
from numbers import Number


@dataclass
class Color:
    r: float
    g: float
    b: float

    def __str__(self):
        r = self.display_component(self.r)
        g = self.display_component(self.g)
        b = self.display_component(self.b)
        return "{} {} {}".format(r, g, b)

    def display_component(self, x: float) -> int:
        return round(clamp(x * 255, 0, 255))

    def debug(self) -> str:
        r = self.r
        g = self.g
        b = self.b
        return "({} {} {})".format(r, g, b)

    def __repr__(self):
        return str(self)

    def __eq__(self, other):
        if not isinstance(other, Color):
            raise NotImplementedError()
        return req(self.r, other.r) and req(self.g, other.g) and req(self.b, other.b)

    def __add__(self, other):
        if not isinstance(other, Color):
            raise NotImplementedError()
        return Color(
            self.r + other.r,
            self.g + other.g,
            self.b + other.b,
        )

    def __sub__(self, other):
        if not isinstance(other, Color):
            raise NotImplementedError()
        return Color(
            self.r - other.r,
            self.g - other.g,
            self.b - other.b,
        )

    def __mul__(self, x):
        if not isinstance(x, Number):
            raise NotImplementedError()
        return Color(
            self.r * x,
            self.g * x,
            self.b * x,
        )

    def blend(self, other):
        if not isinstance(other, Color):
            raise NotImplementedError()
        return Color(
            self.r * other.r,
            self.g * other.g,
            self.b * other.b,
        )
