import math

from rtc.canvas import Canvas
from rtc.color import Color
from rtc.transform import Transform
from rtc.tuples import Point


def demo_transform(*args):
    size = 50
    canvas = Canvas(size, size)
    tick = Color(1, 1, 1)

    for i in range(12):
        p = Point(1, 0, 0)
        r = i * (math.pi / 6)
        T = (
            Transform()
            .rotation_z(r)
            .scaling(size / 4, size / 4, size / 4)
            .translation(size / 2, size / 2, 0)
        )
        Tp = T @ p
        x = round(Tp.x)
        y = round(Tp.y)
        canvas.write(x, y, tick)

    canvas.save("./transform_demo.ppm")
