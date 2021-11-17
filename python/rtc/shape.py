import uuid

from rtc.materials import Material
from rtc.transform import Transform
from rtc.tuples import Vector


class Shape:
    def __init__(self, material=None):
        if material is None:
            material = Material()
        self.id = str(uuid.uuid4())

        self._transform = Transform()
        self.inverse_transform = self.transform.inverse()

        self.material = material

    def __eq__(self, o):
        return self.transform == o.transform and self.material == o.material

    @property
    def transform(self):
        return self._transform

    @transform.setter
    def transform(self, t):
        self._transform = t
        self.inverse_transform = t.inverse()

    def local_intersect(self, ray):
        # For testing
        self.saved_ray = ray

    def intersect(self, r):
        ray = r.transform(self.inverse_transform)
        return self.local_intersect(ray)

    def local_normal_at(self, p):
        return Vector(p.x, p.y, p.z)

    def normal_at(self, world_point):
        local_point = self.inverse_transform @ world_point
        local_normal = self.local_normal_at(local_point)
        world_normal = self.inverse_transform.T @ local_normal
        world_normal.w = 0
        return world_normal.normalize()
