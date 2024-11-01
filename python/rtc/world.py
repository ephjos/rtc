import math

from rtc.color import Color
from rtc.intersection import Intersection, Intersections, Computations
from rtc.lights import PointLight, Light
from rtc.ray import Ray
from rtc.shape import Shape
from rtc.sphere import Sphere
from rtc.transform import *
from rtc.tuples import *

from dataclasses import dataclass
from typing import List

out_ray = Ray(Point(0, 0, 0), Vector(0, 0, 0))
RAY_BOUNCE_DEPTH = 4


@dataclass
class World:
    shapes: List["Shape"]
    lights: List["Light"]

    def intersect(self, ray: "Ray") -> "Intersections":
        intersections: list[Intersection] = []
        for o in self.shapes:
            for i in o.intersect(ray, out_ray):
                intersections.append(i)

        return Intersections(sorted(intersections, key=lambda i: i.t))

    def shade_hit(self, comps: "Computations", remaining: int = RAY_BOUNCE_DEPTH) -> "Color":
        out = Color(0, 0, 0)
        for light in self.lights:
            material = comps.shape.material
            comps.point
            eyev = comps.eyev
            normalv = comps.normalv
            shape = comps.shape

            shadowed = self.is_shadowed(comps.over_point)

            surface = material.lighting(
                shape, light, comps.over_point, eyev, normalv, shadowed
            )
            reflected = self.reflected_color(comps, remaining)
            refracted = self.refracted_color(comps, remaining)

            if material.reflective > 0 and material.transparency > 0:
                reflectance = comps.schlick()
                out += surface + reflected * reflectance + refracted * (1 - reflectance)
                continue

            out += surface + reflected + refracted
        return out

    def color_at(self, ray: "Ray", remaining: int = RAY_BOUNCE_DEPTH) -> "Color":
        intersections = self.intersect(ray)
        hit = intersections.hit()

        if hit is None:
            return Color(0, 0, 0)

        comps = hit.prepare_computations(ray, intersections)
        return self.shade_hit(comps, remaining)

    def is_shadowed(self, point: "Tuple4") -> bool:
        for light in self.lights:
            v = tuple4_sub(light.position, point)
            distance = tuple4_magnitude(v)
            direction = tuple4_normalize(v)

            r = Ray(point, direction)
            intersections = self.intersect(r)

            h = intersections.hit(shadow=True)

            if h is not None and h.t < distance:
                return True

        return False

    def reflected_color(self, comps: "Computations", remaining: int = RAY_BOUNCE_DEPTH) -> "Color":
        reflective = comps.shape.material.reflective
        if reflective == 0 or remaining < 1:
            return Color(0, 0, 0)

        reflect_ray = Ray(comps.over_point, comps.reflectv)
        color = self.color_at(reflect_ray, remaining - 1)
        return color * reflective

    def refracted_color(self, comps, remaining: int = RAY_BOUNCE_DEPTH):
        if remaining == 0:
            return Color(0, 0, 0)
        if comps.shape.material.transparency == 0:
            return Color(0, 0, 0)

        n_ratio = comps.n1 / comps.n2
        cos_i = tuple4_dot(comps.eyev, comps.normalv)
        sin2_t = (n_ratio * n_ratio) * (1 - (cos_i * cos_i))
        if sin2_t > 1:
            return Color(0, 0, 0)

        cos_t = math.sqrt(1 - sin2_t)
        direction = tuple4_sub(
            tuple4_scale(comps.normalv, (n_ratio * cos_i - cos_t)),
            tuple4_scale(comps.eyev, n_ratio),
        )
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
    s2.transform = scaling(0.5, 0.5, 0.5)

    return World([s1, s2], [light])
