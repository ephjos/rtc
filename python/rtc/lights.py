class PointLight:
    def __init__(self, position, intensity):
        self.position = position
        self.intensity = intensity

    def __eq__(self, o):
        return self.position == o.position and self.intensity == o.intensity
