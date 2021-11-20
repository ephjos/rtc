import math

from rtc.color import Color
from rtc.intersection import Intersection, Intersections, Computations
from rtc.lights import PointLight
from rtc.ray import Ray
from rtc.shape import Shape
from rtc.sphere import Sphere
from rtc.transform import Transform
from rtc.tuples import Tuple4, Point

from dataclasses import dataclass


@dataclass
class World:
    shapes: list["Shape"]
    light: "PointLight"

    def intersect(self, ray: "Ray") -> "Intersections":
        intersections: list[Intersection] = []
        for o in self.shapes:
            for i in o.intersect(ray):
                intersections.append(i)

        return Intersections(sorted(intersections, key=lambda i: i.t))

    def shade_hit(self, comps: "Computations", remaining: int = 5) -> "Color":
        material = comps.shape.material
        light = self.light
        point = comps.point
        eyev = comps.eyev
        normalv = comps.normalv
        shape = comps.shape

        shadowed = self.is_shadowed(comps.over_point)

        surface = material.lighting(shape, light, point, eyev, normalv, shadowed)
        reflected = self.reflected_color(comps, remaining)
        refracted = self.refracted_color(comps, remaining)

        if material.reflective > 0 and material.transparency > 0:
            reflectance = comps.schlick()
            return surface + reflected*reflectance + refracted * (1-reflectance)

        return surface + reflected + refracted

    def color_at(self, ray: "Ray", remaining: int = 5) -> "Color":
        intersections = self.intersect(ray)
        hit = intersections.hit()

        if hit is None:
            return Color(0, 0, 0)

        comps = hit.prepare_computations(ray, intersections)
        return self.shade_hit(comps, remaining)

    def is_shadowed(self, point: "Tuple4") -> bool:
        v = self.light.position - point
        distance = v.magnitude()
        direction = v.normalize()

        r = Ray(point, direction)
        intersections = self.intersect(r)

        h = intersections.hit()

        if h is None:
            return False

        return h.shape.cast_shadow and h.t < distance

    def reflected_color(self, comps: "Computations", remaining: int = 5) -> "Color":
        reflective = comps.shape.material.reflective
        if reflective == 0 or remaining < 1:
            return Color(0, 0, 0)

        reflect_ray = Ray(comps.over_point, comps.reflectv)
        color = self.color_at(reflect_ray, remaining - 1)
        return color * reflective

    def refracted_color(self, comps, remaining):
        if remaining == 0:
            return Color(0, 0, 0)
        if comps.shape.material.transparency == 0:
            return Color(0, 0, 0)

        n_ratio = comps.n1 / comps.n2
        cos_i = comps.eyev.dot(comps.normalv)
        sin2_t = (n_ratio * n_ratio) * (1 - (cos_i * cos_i))
        if sin2_t > 1:
            return Color(0, 0, 0)

        cos_t = math.sqrt(1 - sin2_t)
        direction = comps.normalv * (n_ratio * cos_i - cos_t) - comps.eyev * n_ratio
        reflect_ray = Ray(comps.under_point, direction)
        color = (
            self.color_at(reflect_ray, remaining - 1)
            * comps.shape.material.transparency
        )

        return color


def DefaultWorld() -> "World":
    light = PointLight(Point(-10, 10, -10), Color(1, 1, 1))
    s1 = Sphere()
    s1.material.color = Color(0.8, 1.0, 0.6)
    s1.material.diffuse = 0.7
    s1.material.specular = 0.2

    s2 = Sphere()
    s2.transform = Transform().scaling(0.5, 0.5, 0.5)

    return World([s1, s2], light)
