import math

from rtc.camera import Camera
from rtc.color import Color
from rtc.lights import PointLight
from rtc.materials import Material
from rtc.pattern import CheckersPattern, RingPattern, StripePattern
from rtc.plane import Plane
from rtc.sphere import Sphere
from rtc.transform import Transform, ViewTransform
from rtc.tuples import Point, Vector
from rtc.world import World


def demo_refraction(*args):
    floor = Plane()
    floor.material.pattern = CheckersPattern(Color(0.5, 0, 0), Color(1, 1, 1))
    floor.material.reflective = 0.2

    middle = Sphere()
    middle.cast_shadow = False
    middle.transform = Transform().translation(-0.5, 1, 0.5)
    middle.material = Material()
    middle.material.color = Color(0, 0.8, 1.0)
    middle.material.diffuse = 0.7
    middle.material.specular = 0.3
    middle.material.reflective = 0.9
    middle.material.transparency = 0.9

    light = PointLight(Point(-10, 10, -10), Color(1, 1, 1))

    world = World([floor, middle], light)

    s = 10
    camera = Camera(80 * s, 60 * s, math.pi / 3)
    at = Point(0, 1.5, -5)
    to = Point(0, 1, 0)
    up = Vector(0, 1, 0)
    camera.transform = ViewTransform(at, to, up)

    canvas = camera.render(world)
    canvas.save("./demo_refraction.ppm")
