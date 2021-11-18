import math

from rtc.canvas import Canvas
from rtc.ray import Ray
from rtc.transform import Transform
from rtc.tuples import Point
from rtc.world import World


class Camera:
    def __init__(self, hsize: int, vsize: int, fov: float):
        self.hsize = hsize
        self.vsize = vsize
        self.fov = fov
        self.transform = Transform()
        self.inverse_transform = self.transform.inverse()

        half_view = math.tan(fov / 2)
        aspect = hsize / vsize

        if aspect >= 1:
            self.half_width = half_view
            self.half_height = half_view / aspect
        else:
            self.half_width = half_view * aspect
            self.half_height = half_view

        self.pixel_size = (self.half_width * 2) / hsize

    @property
    def transform(self) -> Transform:
        return self._transform

    @transform.setter
    def transform(self, transform):
        self._transform = transform
        self.inverse_transform = transform.inverse()

    def ray_for_pixel(self, px: int, py: int) -> Ray:
        xoffset = (px + 0.5) * self.pixel_size
        yoffset = (py + 0.5) * self.pixel_size

        world_x = self.half_width - xoffset
        world_y = self.half_height - yoffset

        pixel = self.inverse_transform @ Point(world_x, world_y, -1)
        origin = self.inverse_transform @ Point(0, 0, 0)
        direction = (pixel - origin).normalize()

        return Ray(origin, direction)

    def render(self, world: World, progress: bool = False) -> Canvas:
        image = Canvas(self.hsize, self.vsize)

        for y in range(self.vsize):
            for x in range(self.hsize):
                ray = self.ray_for_pixel(x, y)
                color = world.color_at(ray)
                image.write(x, y, color)

        return image
