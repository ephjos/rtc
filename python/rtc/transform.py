import math

from rtc.matrix import Matrix
from rtc.tuples import *


class Transform(Matrix):
    def __init__(self, d: int = 4):
        Matrix.__init__(self, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)
        self.__class__ = Transform

    def inverse(self):
        temp = Matrix.inverse(self)
        self = temp
        self.__class__ = Transform
        return self

    def apply(self, temp):
        self = temp.matmul(self)
        self.__class__ = Transform
        return self

    def translation(self, x: float, y: float, z: float):
        temp = Matrix(1, 0, 0, x, 0, 1, 0, y, 0, 0, 1, z, 0, 0, 0, 1)
        return self.apply(temp)

    def scaling(self, x: float, y: float, z: float):
        temp = Matrix(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1)
        return self.apply(temp)

    def rotation_x(self, r: float):
        temp = Matrix(
            1,
            0,
            0,
            0,
            0,
            math.cos(r),
            -math.sin(r),
            0,
            0,
            math.sin(r),
            math.cos(r),
            0,
            0,
            0,
            0,
            1,
        )
        return self.apply(temp)

    def rotation_y(self, r: float):
        temp = Matrix(
            math.cos(r),
            0,
            math.sin(r),
            0,
            0,
            1,
            0,
            0,
            -math.sin(r),
            0,
            math.cos(r),
            0,
            0,
            0,
            0,
            1,
        )
        return self.apply(temp)

    def rotation_z(self, r: float):
        temp = Matrix(
            math.cos(r),
            -math.sin(r),
            0,
            0,
            math.sin(r),
            math.cos(r),
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
        )
        return self.apply(temp)

    def shearing(
        self, xy: float, xz: float, yx: float, yz: float, zx: float, zy: float
    ):
        temp = Matrix(1, xy, xz, 0, yx, 1, yz, 0, zx, zy, 1, 0, 0, 0, 0, 1)
        return self.apply(temp)


def ViewTransform(at: "Tuple4", to: "Tuple4", up: "Tuple4") -> Transform:
    forward = tuple4_normalize(tuple4_sub(to, at))
    upn = tuple4_normalize(up)
    left = tuple4_cross(forward, upn)
    true_up = tuple4_cross(left, forward)

    orientation = Matrix(
        left[0],
        left[1],
        left[2],
        0,
        true_up[0],
        true_up[1],
        true_up[2],
        0,
        -forward[0],
        -forward[1],
        -forward[2],
        0,
        0,
        0,
        0,
        1,
    )

    return orientation.matmul(Transform().translation(-at[0], -at[1], -at[2]))
