import math

from rtc.matrix import *
from rtc.tuples import *

def translation(x: float, y: float, z: float):
    return Matrix(1, 0, 0, x, 0, 1, 0, y, 0, 0, 1, z, 0, 0, 0, 1)

def scaling(x: float, y: float, z: float):
    return Matrix(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1)

def rotation_x(r: float):
    return Matrix(
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

def rotation_y(r: float):
    return Matrix(
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

def rotation_z(r: float):
    return Matrix(
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

def shearing(
    xy: float, xz: float, yx: float, yz: float, zx: float, zy: float
):
    return Matrix(1, xy, xz, 0, yx, 1, yz, 0, zx, zy, 1, 0, 0, 0, 0, 1)

def ViewTransform(at: "Tuple4", to: "Tuple4", up: "Tuple4") -> Matrix:
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

    return matrix_mul(orientation, translation(-at[0], -at[1], -at[2]))

