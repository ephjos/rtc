from rtc.canvas import Canvas
from rtc.color import Color
from rtc.lights import PointLight
from rtc.ray import Ray
from rtc.sphere import Sphere
from rtc.tuples import *


def demo_material(*args):
    origin = Point(0, 0, -5)
    wall_z = 10
    wall_size = 7
    s = Sphere()
    s.material.color = Color(1, 0.2, 1)

    light_position = Point(-10, -10, -10)
    light_color = Color(1, 1, 1)
    light = PointLight(light_position, light_color)

    canvas_size = 100
    pixel_size = wall_size / canvas_size
    c = Canvas(canvas_size, canvas_size)
    w2 = c.w // 2
    h2 = c.h // 2

    for i in range(-h2, h2):
        world_y = i * pixel_size
        for j in range(-w2, w2):
            world_x = j * pixel_size
            ray = Ray(origin, tuple4_normalize(tuple4_sub(Point(world_x, world_y, wall_z), origin)))
            xs = s.intersect(ray)
            hit = xs.hit()

            if hit is None:
                continue

            point = ray.position(hit.t)
            normal = hit.shape.normal_at(point)
            eye = tuple4_neg(ray.direction)
            color = s.material.lighting(hit.shape, light, point, eye, normal)
            c.write(j + w2, (i + h2), color)

    c.save("./demo_material.ppm")
