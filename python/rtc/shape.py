from rtc.materials import Material
from rtc.ray import Ray
from rtc.matrix import *
from rtc.transform import *
from rtc.tuples import *

from typing import Optional, TYPE_CHECKING

if TYPE_CHECKING:
    from rtc.intersection import Intersections


class Shape:
    def __init__(self, material: Optional[Material] = None):
        if material is None:
            material = Material()
        # self.id = str(uuid.uuid4())

        self._transform = IdentityMatrix()
        self.inverse_transform: Matrix = matrix_inverse(self.transform)

        self.material = material
        self.cast_shadow = True

    def __eq__(self, other):
        if not isinstance(other, Shape):
            raise NotImplementedError()
        return self.transform == other.transform and self.material == other.material

    @property
    def transform(self):
        return self._transform

    @transform.setter
    def transform(self, transform: Matrix):
        self._transform = transform
        self.inverse_transform = matrix_inverse(transform)

    def local_intersect(self, ray: "Ray"):
        # For testing
        self.saved_ray = ray

    def intersect(self, ray: "Ray", out_ray: "Ray" = None) -> "Intersections":
        ray.transform(self.inverse_transform, out_ray)
        return self.local_intersect(out_ray)

    def local_normal_at(self, point: Tuple4) -> Tuple4:
        return Vector(point[0], point[1], point[2])

    def normal_at(self, world_point: Tuple4) -> Tuple4:
        local_point = matrix_mul_tuple(self.inverse_transform, world_point)
        local_normal = self.local_normal_at(local_point)
        world_normal = matrix_mul_tuple(
            matrix_transpose(self.inverse_transform), local_normal
        )
        world_normal[3] = 0.0
        return tuple4_normalize(world_normal)
