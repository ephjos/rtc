from dataclasses import dataclass
from rtc.color import Color
from rtc.tuples import Tuple4


class Light:
    pass


@dataclass
class PointLight(Light):
    position: Tuple4
    intensity: Color

    def __eq__(self, other):
        if not isinstance(other, PointLight):
            raise NotImplementedError()
        return self.position == other.position and self.intensity == other.intensity
