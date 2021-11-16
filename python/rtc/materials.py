from rtc.color import Color
from rtc.utils import req


class Material:
    def __init__(
        self,
        color=Color(1, 1, 1),
        ambient=0.1,
        diffuse=0.9,
        specular=0.9,
        shininess=200.0,
    ):
        self.color = color
        self.ambient = ambient
        self.diffuse = diffuse
        self.specular = specular
        self.shininess = shininess
        self.pattern = None

    def __eq__(self, o):
        return (
            self.color == o.color
            and req(self.ambient, o.ambient)
            and req(self.diffuse, o.diffuse)
            and req(self.specular, o.specular)
            and req(self.shininess, o.shininess)
        )

    def lighting(self, o, light, point, eyev, normalv, in_shadow=False):
        if self.pattern:
            color = self.pattern.pattern_at_shape(o, point)
        else:
            color = self.color

        effective_color = color.blend(light.intensity)
        lightv = (light.position - point).normalize()
        ambient = effective_color * self.ambient

        if in_shadow:
            return ambient

        light_dot_normal = lightv.dot(normalv)

        if light_dot_normal < 0:
            diffuse = Color(0, 0, 0)
            specular = Color(0, 0, 0)
        else:
            diffuse = effective_color * self.diffuse * light_dot_normal

            reflectv = (-lightv).reflect(normalv)
            reflect_dot_eye = reflectv.dot(eyev)

            if reflect_dot_eye <= 0:
                specular = Color(0, 0, 0)
            else:
                factor = pow(reflect_dot_eye, self.shininess)
                specular = light.intensity * self.specular * factor
        return ambient + diffuse + specular
