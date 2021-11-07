

import unittest

from rtc.color import Color
from rtc.lights import PointLight
from rtc.tuples import Point


class TestPointLight(unittest.TestCase):
    def test_point_light_constructor(self):
        intensity = Color(1,1,1)
        position = Point(0,0,0)
        light = PointLight(position, intensity)

        self.assertEqual(light.position, position)
        self.assertEqual(light.intensity, intensity)
