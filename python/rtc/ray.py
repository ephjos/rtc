



class Ray:
    def __init__(self, origin, direction):
        if not origin.isPoint():
            raise Exception("origin must be a point")
        if not direction.isVector():
            raise Exception("direction must be a vector")
        self.origin = origin
        self.direction = direction

    def position(self, t):
        return self.origin + self.direction*t

    def transform(self, t):
        origin = t @ self.origin
        direction = t @ self.direction
        return Ray(origin, direction)

