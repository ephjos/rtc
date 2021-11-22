from rtc.color import Color
from rtc.pattern import Pattern
from rtc.lights import PointLight
from rtc.tuples import *
from rtc.utils import req

from dataclasses import dataclass
from typing import Optional, TYPE_CHECKING

if TYPE_CHECKING:
    from rtc.shape import Shape


@dataclass
class Material:
    color: Color = Color(1, 1, 1)
    ambient: float = 0.1
    diffuse: float = 0.9
    specular: float = 0.9
    shininess: float = 200.0
    reflective: float = 0.0
    pattern: Optional[Pattern] = None
    transparency: float = 0.0
    refractive_index: float = 1.0

    def __eq__(self, other):
        if not isinstance(other, Material):
            raise NotImplementedError()
        return (
            self.color == other.color
            and req(self.ambient, other.ambient)
            and req(self.diffuse, other.diffuse)
            and req(self.specular, other.specular)
            and req(self.shininess, other.shininess)
            and req(self.reflective, other.reflective)
        )

    def lighting(
        self,
        shape: "Shape",
        light: PointLight,
        point: Tuple4,
        eyev: Tuple4,
        normalv: Tuple4,
        in_shadow: bool = False,
    ) -> Color:
        color = self.color
        if self.pattern:
            color = self.pattern.pattern_at_shape(shape, point)

        effective_color = color.blend(light.intensity)
        lightv = tuple4_normalize(tuple4_sub(light.position, point))
        ambient = effective_color * self.ambient

        if in_shadow:
            return ambient

        light_dot_normal = tuple4_dot(lightv, normalv)

        if light_dot_normal < 0:
            diffuse = Color(0, 0, 0)
            specular = Color(0, 0, 0)
        else:
            diffuse = effective_color * self.diffuse * light_dot_normal

            reflectv = tuple4_reflect(tuple4_neg(lightv), normalv)
            reflect_dot_eye = tuple4_dot(reflectv, eyev)

            if reflect_dot_eye <= 0:
                specular = Color(0, 0, 0)
            else:
                factor = pow(reflect_dot_eye, self.shininess)
                specular = light.intensity * self.specular * factor

        return ambient + diffuse + specular
