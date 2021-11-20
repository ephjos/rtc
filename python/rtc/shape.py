from rtc.materials import Material
from rtc.ray import Ray
from rtc.transform import Transform
from rtc.tuples import Tuple4, Vector

from typing import Optional, TYPE_CHECKING, cast

if TYPE_CHECKING:
    from rtc.intersection import Intersections


class Shape:
    def __init__(self, material: Optional[Material] = None):
        if material is None:
            material = Material()
        # self.id = str(uuid.uuid4())

        self._transform = Transform()
        self.inverse_transform: Transform = self.transform.inverse()

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
    def transform(self, transform: Transform):
        self._transform = transform
        self.inverse_transform = transform.inverse()

    def local_intersect(self, ray: "Ray"):
        # For testing
        self.saved_ray = ray

    def intersect(self, ray: "Ray") -> "Intersections":
        ray = ray.transform(self.inverse_transform)
        return self.local_intersect(ray)

    def local_normal_at(self, point: Tuple4) -> Tuple4:
        return Vector(point.x, point.y, point.z)

    def normal_at(self, world_point: Tuple4) -> Tuple4:
        local_point = self.inverse_transform @ world_point
        local_normal = self.local_normal_at(cast(Tuple4, local_point))
        world_normal = cast(Tuple4, self.inverse_transform.T @ local_normal)
        world_normal.w = 0.0
        return world_normal.normalize()
