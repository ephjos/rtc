import math
import unittest

from rtc.color import Color
from rtc.lights import PointLight
from rtc.materials import Material
from rtc.tuples import Point, Vector


class TestMaterial(unittest.TestCase):
    def test_material_constructor(self):
        m = Material()
        self.assertEqual(m.color, Color(1, 1, 1))
        self.assertEqual(m.ambient, 0.1)
        self.assertEqual(m.diffuse, 0.9)
        self.assertEqual(m.specular, 0.9)
        self.assertEqual(m.shininess, 200.0)

    def test_material_lighting_eye_between(self):
        m = Material()
        position = Point(0, 0, 0)

        eyev = Vector(0, 0, -1)
        normalv = Vector(0, 0, -1)
        light = PointLight(Point(0, 0, -10), Color(1, 1, 1))

        result = m.lighting(light, position, eyev, normalv)
        self.assertEqual(result, Color(1.9, 1.9, 1.9))

    def test_material_lighting_eye_offset_45(self):
        m = Material()
        position = Point(0, 0, 0)

        eyev = Vector(0, math.sqrt(2) / 2, -math.sqrt(2) / 2)
        normalv = Vector(0, 0, -1)
        light = PointLight(Point(0, 0, -10), Color(1, 1, 1))

        result = m.lighting(light, position, eyev, normalv)
        self.assertEqual(result, Color(1.0, 1.0, 1.0))

    def test_material_lighting_eye_opposite_light_offset_45(self):
        m = Material()
        position = Point(0, 0, 0)

        eyev = Vector(0, 0, -1)
        normalv = Vector(0, 0, -1)
        light = PointLight(Point(0, 10, -10), Color(1, 1, 1))

        result = m.lighting(light, position, eyev, normalv)
        self.assertEqual(result, Color(0.7364, 0.7364, 0.7364))

    def test_material_lighting_eye_in_reflection(self):
        m = Material()
        position = Point(0, 0, 0)

        eyev = Vector(0, -math.sqrt(2) / 2, -math.sqrt(2) / 2)
        normalv = Vector(0, 0, -1)
        light = PointLight(Point(0, 10, -10), Color(1, 1, 1))

        result = m.lighting(light, position, eyev, normalv)
        self.assertEqual(result, Color(1.6364, 1.6364, 1.6364))

    def test_material_lighting_light_behind(self):
        m = Material()
        position = Point(0, 0, 0)

        eyev = Vector(0, 0, -1)
        normalv = Vector(0, 0, -1)
        light = PointLight(Point(0, 0, 10), Color(1, 1, 1))

        result = m.lighting(light, position, eyev, normalv)
        self.assertEqual(result, Color(0.1, 0.1, 0.1))

    def test_material_lighting_in_shadow(self):
        m = Material()
        position = Point(0, 0, 0)

        eyev = Vector(0, 0, -1)
        normalv = Vector(0, 0, -1)
        light = PointLight(Point(0, 0, -10), Color(1, 1, 1))
        in_shadow = True

        result = m.lighting(light, position, eyev, normalv, in_shadow)
        self.assertEqual(result, Color(0.1, 0.1, 0.1))
