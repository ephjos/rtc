import math

from rtc.canvas import Canvas
from rtc.color import Color
from rtc.matrix import *
from rtc.transform import *
from rtc.tuples import Point


def demo_transform(*args):
    size = 50
    canvas = Canvas(size, size)
    tick = Color(1, 1, 1)

    for i in range(12):
        p = Point(1, 0, 0)
        r = i * (math.pi / 6)
        T = (
            matrix_mul(translation(size / 2, size / 2, 0),
                matrix_mul(rotation_z(r),
                    scaling(size / 4, size / 4, size / 4)))
        )
        Tp = matrix_mul_tuple(T, p)
        x = round(Tp[0])
        y = round(Tp[1])
        canvas.write(x, y, tick)

    canvas.save("./demo_transform.ppm")
