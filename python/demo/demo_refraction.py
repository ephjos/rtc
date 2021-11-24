from rtc.camera import Camera
from rtc.color import Color
from rtc.lights import PointLight
from rtc.materials import Material
from rtc.pattern import CheckersPattern, StripePattern
from rtc.plane import Plane
from rtc.sphere import Sphere
from rtc.matrix import *
from rtc.transform import *
from rtc.tuples import Point, Vector
from rtc.world import World


def demo_refraction(*args):
    camera = Camera(40, 20, 1.152)
    camera.transform = ViewTransform(
        Point(-2.6, 1.5, -3.9), Point(-0.6, 1, -0.8), Vector(0, 1, 0)
    )

    light = PointLight(Point(-4.9, 4.9, -1), Color(1, 1, 1))

    wall_material = Material()
    wall_material.pattern = StripePattern(
        Color(0.45, 0.45, 0.45), Color(0.55, 0.55, 0.55)
    )
    wall_material.pattern.transform = matrix_mul(
        scaling(0.25, 0.25, 0.25), rotation_y(1.5708)
    )
    wall_material.ambient = 0
    wall_material.diffuse = 0.4
    wall_material.specular = 0
    wall_material.reflective = 0.3

    floor = Plane()
    floor.transform = rotation_y(0.31415)
    floor.material.pattern = CheckersPattern(
        Color(0.35, 0.35, 0.35), Color(0.65, 0.65, 0.65)
    )
    floor.material.specular = 0
    floor.material.reflective = 0.4

    ceiling = Plane()
    ceiling.material.color = Color(0.8, 0.8, 0.8)
    ceiling.material.ambient = 0.3
    ceiling.material.specular = 0

    west_wall = Plane()
    west_wall.material = wall_material
    west_wall.transform = matrix_mul(
        translation(-5, 0, 0), matrix_mul(rotation_y(1.5708), rotation_z(1.5708))
    )

    east_wall = Plane()
    east_wall.material = wall_material
    east_wall.transform = matrix_mul(
        translation(5, 0, 0), matrix_mul(rotation_y(1.5708), rotation_z(1.5708))
    )

    north_wall = Plane()
    north_wall.material = wall_material
    north_wall.transform = matrix_mul(translation(0, 0, 5), rotation_x(1.5708))

    south_wall = Plane()
    south_wall.material = wall_material
    south_wall.transform = matrix_mul(translation(0, 0, -5), rotation_x(1.5708))

    background_sphere_1 = Sphere()
    background_sphere_1.transform = matrix_mul(
        translation(4.6, 0.4, 1), scaling(0.4, 0.4, 0.4)
    )
    background_sphere_1.material.color = Color(0.8, 0.5, 0.3)
    background_sphere_1.material.shininess = 50

    background_sphere_2 = Sphere()
    background_sphere_2.transform = matrix_mul(
        translation(4.7, 0.3, 4), scaling(0.3, 0.3, 0.3)
    )
    background_sphere_2.material.color = Color(0.9, 0.4, 0.5)
    background_sphere_2.material.shininess = 50

    background_sphere_3 = Sphere()
    background_sphere_3.transform = matrix_mul(
        translation(-1, 0.5, 4.5), scaling(0.5, 0.5, 0.5)
    )
    background_sphere_3.material.color = Color(0.4, 0.9, 0.6)
    background_sphere_3.material.shininess = 50

    background_sphere_4 = Sphere()
    background_sphere_4.transform = matrix_mul(
        translation(-1.7, 0.3, 4.7), scaling(0.3, 0.3, 0.3)
    )
    background_sphere_4.material.color = Color(0.4, 0.6, 0.9)
    background_sphere_4.material.shininess = 50

    red_sphere = Sphere()
    red_sphere.transform = translation(-0.6, 1, 0.6)
    red_sphere.material.color = Color(1, 0.3, 0.2)
    red_sphere.material.specular = 0.4
    red_sphere.material.shininess = 50

    blue_glass_sphere = Sphere()
    blue_glass_sphere.transform = matrix_mul(
        translation(0.6, 0.7, -0.6), scaling(0.7, 0.7, 0.7)
    )
    blue_glass_sphere.material.color = Color(0, 0, 0.2)
    blue_glass_sphere.material.ambient = 0
    blue_glass_sphere.material.diffuse = 0.4
    blue_glass_sphere.material.specular = 0.9
    blue_glass_sphere.material.shininess = 300
    blue_glass_sphere.material.reflective = 0.9
    blue_glass_sphere.material.transparency = 0.9
    blue_glass_sphere.material.refractive_index = 1.5

    green_glass_sphere = Sphere()
    green_glass_sphere.transform = matrix_mul(
        translation(-0.7, 0.5, -0.8), scaling(0.5, 0.5, 0.5)
    )
    green_glass_sphere.material.color = Color(0, 0.2, 0)
    green_glass_sphere.material.ambient = 0
    green_glass_sphere.material.diffuse = 0.4
    green_glass_sphere.material.specular = 0.9
    green_glass_sphere.material.shininess = 300
    green_glass_sphere.material.reflective = 0.9
    green_glass_sphere.material.transparency = 0.9
    green_glass_sphere.material.refractive_index = 1.5

    world = World(
        [
            floor,
            ceiling,
            west_wall,
            east_wall,
            north_wall,
            south_wall,
            background_sphere_1,
            background_sphere_2,
            background_sphere_3,
            background_sphere_4,
            red_sphere,
            blue_glass_sphere,
            green_glass_sphere,
        ],
        [light],
    )

    canvas = camera.render(world)
    canvas.save("./demo_refraction.ppm")
