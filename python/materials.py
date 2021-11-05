#!/usr/bin/env python

import math
import unittest

from color import Color
from lights import PointLight
from tuples import Tuple, Point, Vector
from utils import req

class Material:
    def __init__(self,
                 color=Color(1,1,1),
                 ambient=0.1,
                 diffuse=0.9,
                 specular=0.9,
                 shininess=200.0):
        self.color = color
        self.ambient = ambient
        self.diffuse = diffuse
        self.specular = specular
        self.shininess = shininess

    def __eq__(self, o):
        return self.color == o.color and \
            req(self.ambient, o.ambient) and \
            req(self.diffuse, o.diffuse) and \
            req(self.specular, o.specular) and \
            req(self.shininess, o.shininess)

    def lighting(self, light, point, eyev, normalv):
        effective_color = self.color.blend(light.intensity)
        lightv = (light.position - point).normalize()
        ambient = effective_color * self.ambient
        light_dot_normal = lightv.dot(normalv)

        diffuse = Color(0,0,0)
        specular = Color(0,0,0)

        if light_dot_normal >= 0:
            diffuse = effective_color * self.diffuse * light_dot_normal

        reflectv = (-lightv).reflect(normalv)
        reflect_dot_eye = reflectv.dot(eyev)

        if reflect_dot_eye >= 0:
            factor = pow(reflect_dot_eye, self.shininess)
            specular = light.intensity * self.specular * factor

        return ambient + diffuse + specular


class TestMaterial(unittest.TestCase):
    def test_material_constructor(self):
        m = Material()
        self.assertEqual(m.color, Color(1,1,1))
        self.assertEqual(m.ambient, 0.1)
        self.assertEqual(m.diffuse, 0.9)
        self.assertEqual(m.specular, 0.9)
        self.assertEqual(m.shininess, 200.0)

    def test_material_lighting_eye_between(self):
        m = Material()
        position = Point(0,0,0)

        eyev = Vector(0,0,-1)
        normalv = Vector(0,0,-1)
        light = PointLight(Point(0,0,-10), Color(1,1,1))

        result = m.lighting(light, position, eyev, normalv)
        self.assertEqual(result, Color(1.9,1.9,1.9))

    def test_material_lighting_eye_offset_45(self):
        m = Material()
        position = Point(0,0,0)

        eyev = Vector(0,math.sqrt(2)/2,-math.sqrt(2)/2)
        normalv = Vector(0,0,-1)
        light = PointLight(Point(0,0,-10), Color(1,1,1))

        result = m.lighting(light, position, eyev, normalv)
        self.assertEqual(result, Color(1.0,1.0,1.0))

    def test_material_lighting_eye_opposite_light_offset_45(self):
        m = Material()
        position = Point(0,0,0)

        eyev = Vector(0,0,-1)
        normalv = Vector(0,0,-1)
        light = PointLight(Point(0,10,-10), Color(1,1,1))

        result = m.lighting(light, position, eyev, normalv)
        self.assertEqual(result, Color(0.7364, 0.7364, 0.7364))

    def test_material_lighting_eye_in_reflection(self):
        m = Material()
        position = Point(0,0,0)

        eyev = Vector(0,-math.sqrt(2)/2,-math.sqrt(2)/2)
        normalv = Vector(0,0,-1)
        light = PointLight(Point(0,10,-10), Color(1,1,1))

        result = m.lighting(light, position, eyev, normalv)
        self.assertEqual(result, Color(1.6364,1.6364,1.6364))

    def test_material_lighting_light_behind(self):
        m = Material()
        position = Point(0,0,0)

        eyev = Vector(0,0,-1)
        normalv = Vector(0,0,-1)
        light = PointLight(Point(0,0,10), Color(1,1,1))

        result = m.lighting(light, position, eyev, normalv)
        self.assertEqual(result, Color(0.1,0.1,0.1))

