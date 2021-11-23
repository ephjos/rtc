import array
import math

from rtc.utils import req


def Tuple4(x: float, y: float, z: float, w: float):
    return array.array("f", [x, y, z, w])


def Point(x: float, y: float, z: float):
    return Tuple4(x, y, z, 1)


def Vector(x: float, y: float, z: float):
    return Tuple4(x, y, z, 0)


def tuple4_is_point(this):
    return req(this[3], 1)


def tuple4_is_vector(this):
    return req(this[3], 0)


def tuple4_eq(this, other):
    return (
        req(this[0], other[0])
        and req(this[1], other[1])
        and req(this[2], other[2])
        and req(this[3], other[3])
    )


def tuple4_add(this, other):
    return Tuple4(
        this[0] + other[0],
        this[1] + other[1],
        this[2] + other[2],
        this[3] + other[3],
    )


def tuple4_sub(this, other):
    return Tuple4(
        this[0] - other[0],
        this[1] - other[1],
        this[2] - other[2],
        this[3] - other[3],
    )


def tuple4_neg(this):
    return Tuple4(
        -this[0],
        -this[1],
        -this[2],
        -this[3],
    )


def tuple4_scale(this, other: float):
    return Tuple4(
        this[0] * other,
        this[1] * other,
        this[2] * other,
        this[3] * other,
    )


def tuple4_divide(this, other: float):
    return Tuple4(
        this[0] / other,
        this[1] / other,
        this[2] / other,
        this[3] / other,
    )


def tuple4_magnitude(this):
    return math.sqrt((this[0] ** 2) + (this[1] ** 2) + (this[2] ** 2) + (this[3] ** 2))


def tuple4_normalize(this):
    return tuple4_divide(this, tuple4_magnitude(this))


def tuple4_dot(this, other):
    return (
        this[0] * other[0]
        + this[1] * other[1]
        + this[2] * other[2]
        + this[3] * other[3]
    )


def tuple4_cross(this, other):
    return Vector(
        this[1] * other[2] - this[2] * other[1],
        this[2] * other[0] - this[0] * other[2],
        this[0] * other[1] - this[1] * other[0],
    )


def tuple4_reflect(this, normal):
    x = tuple4_scale(normal, 2 * tuple4_dot(this, normal))
    return tuple4_sub(this, x)
