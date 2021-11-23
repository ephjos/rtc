from rtc.canvas import Canvas
from rtc.color import Color
from rtc.ray import Ray
from rtc.sphere import Sphere
from rtc.tuples import *


def demo_ray(*args):
    origin = Point(0, 0, -5)
    wall_z = 10
    wall_size = 7
    s = Sphere()

    hit_color = Color(1, 0, 0)

    canvas_size = 100
    pixel_size = wall_size / canvas_size
    c = Canvas(canvas_size, canvas_size)
    w2 = c.w // 2
    h2 = c.h // 2

    for i in range(-h2, h2):
        world_y = i * pixel_size
        for j in range(-w2, w2):
            world_x = j * pixel_size
            ray = Ray(
                origin,
                tuple4_normalize(tuple4_sub(Point(world_x, world_y, wall_z), origin)),
            )
            xs = s.intersect(ray)
            hit = xs.hit()

            if hit is None:
                continue
            c.write(i + h2, j + w2, hit_color)

    c.save("./demo_ray.ppm")
