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


def demo_reflection(*args):
    floor = Plane()
    floor.material.pattern = CheckersPattern(Color(0.5, 0, 0), Color(1, 1, 1))
    floor.material.reflective = 0.2

    middle = Sphere()
    middle.transform = Transform().translation(-0.5, 1, 0.5)
    middle.material = Material()
    middle.material.pattern = RingPattern(Color(0, 0, 0), Color(1, 1, 1))
    middle.material.pattern.transform = (
        Transform().rotation_x(math.pi / 2).scaling(0.2, 0.2, 0.2)
    )
    middle.material.color = Color(0.1, 1, 0.5)
    middle.material.diffuse = 0.7
    middle.material.specular = 0.3
    middle.material.reflective = 0.5

    right = Sphere()
    right.transform = Transform().scaling(0.5, 0.5, 0.5).translation(1.5, 0.5, -0.5)
    right.material = Material()
    right.material.pattern = StripePattern(Color(0, 0.2, 0.7), Color(1, 0.7, 0.1))
    right.material.pattern.transform = (
        Transform().rotation_z(math.pi / 2).scaling(2, 0.5, 1)
    )
    right.material.color = Color(0.5, 1, 0.1)
    right.material.diffuse = 0.7
    right.material.specular = 0.3
    right.material.reflective = 0.01

    left = Sphere()
    left.transform = (
        Transform().scaling(0.33, 0.33, 0.33).translation(-1.5, 0.33, -0.75)
    )
    left.material = Material()
    left.material.pattern = CheckersPattern(Color(0.0, 0.7, 0.2), Color(0.5, 0.5, 0.0))
    left.material.pattern.transform = (
        Transform().rotation_y(math.pi / 6).translation(2, 0.5, 1)
    )
    left.material.color = Color(1, 0.8, 0.1)
    left.material.diffuse = 0.7
    left.material.diffuse = 0.3
    left.material.reflective = 0.2

    light = PointLight(Point(-10, 10, -10), Color(1, 1, 1))

    world = World([floor, middle, right, left], [light])

    s = 1
    camera = Camera(80 * s, 60 * s, math.pi / 3)
    at = Point(0, 1.5, -5)
    to = Point(0, 1, 0)
    up = Vector(0, 1, 0)
    camera.transform = ViewTransform(at, to, up)

    canvas = camera.render(world)
    canvas.save("./demo_reflection.ppm")
