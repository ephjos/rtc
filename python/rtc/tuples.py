import math

from rtc.utils import req

from dataclasses import dataclass


@dataclass
class Tuple4:
    x: float
    y: float
    z: float
    w: float

    def isPoint(self):
        return req(self.w, 1)

    def isVector(self):
        return req(self.w, 0)

    def __str__(self):
        return "[{:6.2f} {:6.2f} {:6.2f} {:6.2f}]".format(
            self.x, self.y, self.z, self.w
        )

    def __repr__(self):
        return str(self)

    def __eq__(self, other):
        if not isinstance(other, Tuple4):
            raise NotImplementedError()
        return (
            req(self.x, other.x)
            and req(self.y, other.y)
            and req(self.z, other.z)
            and req(self.w, other.w)
        )

    def __add__(self, other):
        if not isinstance(other, Tuple4):
            raise NotImplementedError()
        return Tuple4(
            self.x + other.x,
            self.y + other.y,
            self.z + other.z,
            self.w + other.w,
        )

    def __sub__(self, other):
        if not isinstance(other, Tuple4):
            raise NotImplementedError()
        return Tuple4(
            self.x - other.x,
            self.y - other.y,
            self.z - other.z,
            self.w - other.w,
        )

    def __neg__(self):
        return Tuple4(
            -self.x,
            -self.y,
            -self.z,
            -self.w,
        )

    def __mul__(self, other: float):
        return Tuple4(
            self.x * other,
            self.y * other,
            self.z * other,
            self.w * other,
        )

    def __truediv__(self, other: float):
        return Tuple4(
            self.x / other,
            self.y / other,
            self.z / other,
            self.w / other,
        )

    def magnitude(self):
        return math.sqrt((self.x ** 2) + (self.y ** 2) + (self.z ** 2) + (self.w ** 2))

    def normalize(self):
        return self / self.magnitude()

    def dot(self, other):
        if not isinstance(other, Tuple4):
            raise NotImplementedError()
        return self.x * other.x + self.y * other.y + self.z * other.z + self.w * other.w

    def cross(self, other):
        if not isinstance(other, Tuple4):
            raise NotImplementedError()
        return Vector(
            self.y * other.z - self.z * other.y,
            self.z * other.x - self.x * other.z,
            self.x * other.y - self.y * other.x,
        )

    def reflect(self, normal):
        if not isinstance(normal, Tuple4):
            raise NotImplementedError()
        return self - normal * 2 * self.dot(normal)


def Point(x: float, y: float, z: float):
    return Tuple4(x, y, z, 1)


def Vector(x: float, y: float, z: float):
    return Tuple4(x, y, z, 0)
