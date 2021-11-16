from rtc.color import Color
from rtc.intersection import Intersections, Computations
from rtc.lights import PointLight
from rtc.ray import Ray
from rtc.sphere import Sphere
from rtc.transform import Transform
from rtc.tuples import Point


class World:
    def __init__(self, objects=None, light=None):
        if objects is None:
            objects = []
        self.objects = objects
        self.light = light

    def intersect(self, ray):
        intersections = []
        for o in self.objects:
            intersections += o.intersect(ray)

        return Intersections(sorted(intersections, key=lambda i: i.t))

    def shade_hit(self, comps: Computations):
        material = comps.object.material
        light = self.light
        point = comps.point
        eyev = comps.eyev
        normalv = comps.normalv
        o = comps.object

        shadowed = self.is_shadowed(comps.over_point)

        return material.lighting(o, light, point, eyev, normalv, shadowed)

    def color_at(self, r):
        intersections = self.intersect(r)
        hit = intersections.hit()

        if hit is None:
            return Color(0, 0, 0)

        comps = hit.prepare_computations(r)
        return self.shade_hit(comps)

    def is_shadowed(self, point):
        v = self.light.position - point
        distance = v.magnitude()
        direction = v.normalize()

        r = Ray(point, direction)
        intersections = self.intersect(r)

        h = intersections.hit()

        if h is None:
            return False

        return h.t < distance


def DefaultWorld():
    light = PointLight(Point(-10, 10, -10), Color(1, 1, 1))
    s1 = Sphere()
    s1.material.color = Color(0.8, 1.0, 0.6)
    s1.material.diffuse = 0.7
    s1.material.specular = 0.2

    s2 = Sphere()
    s2.transform = Transform().scaling(0.5, 0.5, 0.5)

    return World([s1, s2], light)
