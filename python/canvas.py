#!/usr/bin/env python
import math
import unittest

from color import Color
from tuples import Vector, Point
from utils import req

class Canvas:
    def __init__(self, w, h):
        self.w = w
        self.h = h
        self.p = [Color(0,0,0) for _ in range(w*h)]

    def write(self, x, y, color):
        self.p[x + (y * self.w)] = color

    def read(self, x, y):
        return self.p[x + (y * self.w)]

    def ppm(self):
        lines = ["P3", "{} {}".format(self.w, self.h), "255", ""]

        l = self.w*self.h
        s = 5
        for i in range(0, l, s):
            line = ' '.join(map(str, self.p[i:min(i+s, l)]))
            lines.append(line)

        return "\n".join(lines) + "\n"

    def save(self, filename):
        with open(filename, "w") as savefile:
            savefile.write("P3\n{} {}\n255\n\n".format(self.w, self.h))
            l = self.w*self.h
            s = 50
            for i in range(0, l, s):
                line = ' '.join(map(str, self.p[i:min(i+s, l)]))
                savefile.write(line)
                savefile.write("\n")

            savefile.write("\n")


class TestCanvas(unittest.TestCase):
    def test_canvas_constructor(self):
        c = Canvas(10, 20)
        self.assertEqual(c.w, 10)
        self.assertEqual(c.h, 20)
        for p in c.p:
            self.assertEqual(p, Color(0,0,0))

    def test_write_to_canvas(self):
        c = Canvas(10, 20)
        red = Color(1, 0, 0)
        c.write(2, 3, red)
        self.assertEqual(c.read(2, 3), red)

    def test_ppm_header(self):
        c = Canvas(5, 3)
        self.assertEqual("\n".join(c.ppm().split("\n")[:4]), "P3\n5 3\n255\n")

    def test_ppm(self):
        c = Canvas(5, 3)
        c.write(0, 0, Color(1.5, 0, 0))
        c.write(2, 1, Color(0, 0.5, 0))
        c.write(4, 2, Color(-0.5, 0, 1))

        ppm = c.ppm().split("\n")
        line1 = ppm[4]
        line2 = ppm[5]
        line3 = ppm[6]

        self.assertEqual(line1, "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0")
        self.assertEqual(line2, "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0")
        self.assertEqual(line3, "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255")

    def test_ppm_end_newline(self):
        c = Canvas(5, 3)
        self.assertEqual(c.ppm()[-1], "\n")

def demo_canvas(*args):
    class Projectile:
        def __init__(self, position, velocity):
            self.position = position
            self.velocity = velocity

    class Environment:
        def __init__(self, gravity, wind):
            self.gravity = gravity
            self.wind = wind

    def tick(env,prj):
        position = prj.position + prj.velocity
        velocity = prj.velocity + env.gravity + env.wind
        return Projectile(position, velocity)

    def print_prj(p):
        print("{:6.2f} {:6.2f}".format(p.position.x, p.position.y))

    p = Projectile(Point(0,1,0), Vector(1,1.8,0).normalize() * 11.25)
    e = Environment(Vector(0,-0.1,0), Vector(-0.01,0,0))

    c = Canvas(900, 550)
    trail = Color(1,0,0)

    while p.position.y > 0:
        print_prj(p)
        p = tick(e,p)
        x = round(p.position.x)
        y = round(p.position.y)
        if x >= 0 and x < c.w and y >= 0 and y < c.h:
            c.write(x, c.h-y, trail)

    print("Hit!")
    print_prj(p)

    with open("./canvas_demo.ppm", "w") as outfile:
        outfile.write(c.ppm())
