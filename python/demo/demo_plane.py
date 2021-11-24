import math

from rtc.camera import Camera
from rtc.color import Color
from rtc.lights import PointLight
from rtc.materials import Material
from rtc.plane import Plane
from rtc.sphere import Sphere
from rtc.matrix import *
from rtc.transform import *
from rtc.tuples import Point, Vector
from rtc.world import World


def demo_plane(*args):
    floor = Plane()

    middle = Sphere()
    middle.transform = translation(-0.5, 1, 0.5)
    middle.material = Material()
    middle.material.color = Color(0.1, 1, 0.5)
    middle.material.diffuse = 0.7
    middle.material.specular = 0.3

    right = Sphere()
    right.transform = matrix_mul(translation(1.5, 0.5, -0.5), scaling(0.5, 0.5, 0.5))
    right.material = Material()
    right.material.color = Color(0.5, 1, 0.1)
    right.material.diffuse = 0.7
    right.material.specular = 0.3

    left = Sphere()
    left.transform = matrix_mul(
        translation(-1.5, 0.33, -0.75), scaling(0.33, 0.33, 0.33)
    )
    left.material = Material()
    left.material.color = Color(1, 0.8, 0.1)
    left.material.diffuse = 0.7
    left.material.diffuse = 0.3

    light = PointLight(Point(-10, 10, -10), Color(1, 1, 1))

    world = World([floor, middle, right, left], [light])

    camera = Camera(80, 60, math.pi / 3)
    at = Point(0, 1.5, -5)
    to = Point(0, 1, 0)
    up = Vector(0, 1, 0)
    camera.transform = ViewTransform(at, to, up)

    canvas = camera.render(world)
    canvas.save("./demo_plane.ppm")
