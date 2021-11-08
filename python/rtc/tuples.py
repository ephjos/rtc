import math

from rtc.utils import req


class Tuple:
    def __init__(self, x, y, z, w):
        self.x = x
        self.y = y
        self.z = z
        self.w = w

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

    def __eq__(self, o):
        return (
            req(self.x, o.x)
            and req(self.y, o.y)
            and req(self.z, o.z)
            and req(self.w, o.w)
        )

    def __add__(self, o):
        return Tuple(
            self.x + o.x,
            self.y + o.y,
            self.z + o.z,
            self.w + o.w,
        )

    def __sub__(self, o):
        return Tuple(
            self.x - o.x,
            self.y - o.y,
            self.z - o.z,
            self.w - o.w,
        )

    def __neg__(self):
        return Tuple(
            -self.x,
            -self.y,
            -self.z,
            -self.w,
        )

    def __mul__(self, o):
        return Tuple(
            self.x * o,
            self.y * o,
            self.z * o,
            self.w * o,
        )

    def __truediv__(self, o):
        return Tuple(
            self.x / o,
            self.y / o,
            self.z / o,
            self.w / o,
        )

    def magnitude(self):
        return math.sqrt((self.x ** 2) + (self.y ** 2) + (self.z ** 2) + (self.w ** 2))

    def normalize(self):
        return self / self.magnitude()

    def dot(self, o):
        return self.x * o.x + self.y * o.y + self.z * o.z + self.w * o.w

    def cross(self, o):
        return Vector(
            self.y * o.z - self.z * o.y,
            self.z * o.x - self.x * o.z,
            self.x * o.y - self.y * o.x,
        )

    def reflect(self, normal):
        return self - normal * 2 * self.dot(normal)


def Point(x, y, z):
    return Tuple(x, y, z, 1)


def Vector(x, y, z):
    return Tuple(x, y, z, 0)
