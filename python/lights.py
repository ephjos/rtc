#!/usr/bin/env python

import math
import unittest

from color import Color
from tuples import Tuple, Point, Vector

class PointLight:
    def __init__(self, position, intensity):
        self.position = position
        self.intensity = intensity

class TestPointLight(unittest.TestCase):
    def test_point_light_constructor(self):
        intensity = Color(1,1,1)
        position = Point(0,0,0)
        light = PointLight(position, intensity)

        self.assertEqual(light.position, position)
        self.assertEqual(light.intensity, intensity)
