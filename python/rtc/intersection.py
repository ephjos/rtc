
from rtc.utils import EPSILON

class Computations:
    def __init__(self):
        self.t = None
        self.object = None
        self.point = None
        self.eyev = None
        self.normalv = None
        self.inside = None
        self.over_point = None

class Intersection:
    def __init__(self, t, o):
        self.t = t
        self.object = o

    def prepare_computations(self, ray):
        comps = Computations()
        comps.t = self.t
        comps.object = self.object
        comps.point = ray.position(self.t)
        comps.eyev = -ray.direction
        comps.normalv = self.object.normal_at(comps.point)
        comps.inside = False

        if comps.normalv.dot(comps.eyev) < 0:
            comps.inside = True
            comps.normalv = -comps.normalv

        comps.over_point = comps.point + comps.normalv * EPSILON

        return comps

class Intersections:
    def __init__(self, intersections=None):
        if intersections is None:
            intersections = []
        self.intersections = intersections

    def __getitem__(self, index):
        return self.intersections[index]

    def __len__(self):
        return len(self.intersections)

    def hit(self):
        if len(self) == 0:
            return None

        positive_intersections = list(filter(lambda i: i.t >= 0, self.intersections))
        if len(positive_intersections) == 0:
            return None

        curr = positive_intersections[0]
        for i in positive_intersections[1:]:
            if i.t > 0 and i.t < curr.t:
                curr = i

        if curr.t < 0:
            return None
        return curr
