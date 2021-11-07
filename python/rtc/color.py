

from rtc.utils import req, clamp

class Color:
    def __init__(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b

    def __str__(self):
        display_color = lambda c: clamp(round(c*255), 0, 255)
        r = display_color(self.r)
        g = display_color(self.g)
        b = display_color(self.b)
        return "{} {} {}".format(r, g, b)

    def debug(self):
        r = self.r
        g = self.g
        b = self.b
        return "({} {} {})".format(r, g, b)

    def __repr__(self):
        return str(self)

    def __eq__(self, o):
        return req(self.r,o.r) and req(self.g,o.g) and req(self.b,o.b)

    def __add__(self, o):
        return Color(
            self.r + o.r,
            self.g + o.g,
            self.b + o.b,
        )

    def __sub__(self, o):
        return Color(
            self.r - o.r,
            self.g - o.g,
            self.b - o.b,
        )

    def __mul__(self, s):
        return Color(
            self.r * s,
            self.g * s,
            self.b * s,
        )

    def blend(self, o):
        return Color(
            self.r * o.r,
            self.g * o.g,
            self.b * o.b,
        )


