
import math

from rtc.camera import Camera
from rtc.canvas import Canvas
from rtc.color import Color
from rtc.lights import PointLight
from rtc.materials import Material
from rtc.sphere import Sphere
from rtc.transform import Transform, ViewTransform
from rtc.tuples import Point, Vector
from rtc.world import World

def demo_camera(*args):
    floor = Sphere()
    floor.transform = Transform().scaling(10,0.01,10)
    floor.material = Material()
    floor.material.color = Color(1,0.9,0.9)
    floor.material.specular = 0

    left_wall = Sphere()
    left_wall.transform = Transform().scaling(10,0.01,10).rotation_x(math.pi/2).rotation_y(-math.pi/4).translation(0,0,5)
    left_wall.material = floor.material

    right_wall = Sphere()
    right_wall.transform = Transform().scaling(10,0.01,10).rotation_x(math.pi/2).rotation_y(math.pi/4).translation(0,0,5)
    right_wall.material = floor.material

    middle = Sphere()
    middle.transform = Transform().translation(-0.5, 1, 0.5)
    middle.material = Material()
    middle.material.color = Color(0.1,1,0.5)
    middle.material.diffuse = 0.7
    middle.material.specular = 0.3

    right = Sphere()
    right.transform = Transform().scaling(0.5,0.5,0.5).translation(1.5,0.5,-0.5)
    right.material = Material()
    right.material.color = Color(0.5,1,0.1)
    right.material.diffuse = 0.7
    right.material.specular = 0.3

    left = Sphere()
    left.transform = Transform().scaling(0.33,0.33,0.33).translation(-1.5, 0.33, -0.75)
    left.material = Material()
    left.material.color = Color(1,0.8,0.1)
    left.material.diffuse = 0.7
    left.material.diffuse = 0.3

    light = PointLight(Point(-10,10,-10), Color(1,1,1))

    world = World([floor, left_wall, right_wall, middle, right, left], light)

    camera = Camera(80, 60, math.pi/3)
    at = Point(0,1.5,-5)
    to = Point(0,1,0)
    up = Vector(0,1,0)
    camera.transform = ViewTransform(at,to,up)

    canvas = camera.render(world)
    canvas.save("./demo_camera.ppm")
