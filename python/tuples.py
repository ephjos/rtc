#!/usr/bin/env python
import math
import unittest

from utils import req

class Tuple:
    def __init__(self,x,y,z,w):
        self.x = x
        self.y = y
        self.z = z
        self.w = w

    def isPoint(self):
        return req(self.w,1)

    def isVector(self):
        return req(self.w,0)

    def __str__(self):
        return "[{:6.2f} {:6.2f} {:6.2f} {:6.2f}]".format(self.x,self.y,self.z,self.w)

    def __repr__(self):
        return str(self)

    def __eq__(self, o):
        return req(self.x,o.x) and req(self.y,o.y) and req(self.z,o.z) and req(self.w,o.w)

    def __add__(self, o):
        return Tuple(
            self.x + o.x,
            self.y + o.y,
            self.z + o.z,
            self.w + o.w,
        )

    def __sub__(self, o):
        return Tuple(
            self.x - o.x,
            self.y - o.y,
            self.z - o.z,
            self.w - o.w,
        )

    def __neg__(self):
        return Tuple(
            -self.x,
            -self.y,
            -self.z,
            -self.w,
        )

    def __mul__(self, o):
        return Tuple(
            self.x * o,
            self.y * o,
            self.z * o,
            self.w * o,
        )

    def __truediv__(self, o):
        return Tuple(
            self.x / o,
            self.y / o,
            self.z / o,
            self.w / o,
        )

    def magnitude(self):
        return math.sqrt(
            (self.x ** 2) +
            (self.y ** 2) +
            (self.z ** 2) +
            (self.w ** 2)
        )

    def normalize(self):
        return self / self.magnitude()

    def dot(self, o):
        return self.x * o.x + self.y * o.y + self.z * o.z + self.w * o.w

    def cross(self, o):
        return Vector(
            self.y*o.z - self.z*o.y,
            self.z*o.x - self.x*o.z,
            self.x*o.y - self.y*o.x,
        )

    def reflect(self, normal):
        return self - normal * 2 * self.dot(normal)

def Point(x,y,z):
    return Tuple(x,y,z,1)

def Vector(x,y,z):
    return Tuple(x,y,z,0)


class TestTuples(unittest.TestCase):
    def test_tuple_w_1(self):
        a = Tuple(4.3,-4.2,3.1,1.0)
        self.assertEqual(a.x,4.3)
        self.assertEqual(a.y,-4.2)
        self.assertEqual(a.z,3.1)
        self.assertEqual(a.w,1.0)
        self.assertTrue(a.isPoint())
        self.assertFalse(a.isVector())

    def test_tuple_w_0(self):
        a = Tuple(4.3,-4.2,3.1,0.0)
        self.assertEqual(a.x,4.3)
        self.assertEqual(a.y,-4.2)
        self.assertEqual(a.z,3.1)
        self.assertEqual(a.w,0.0)
        self.assertFalse(a.isPoint())
        self.assertTrue(a.isVector())

    def test_point_creates(self):
        p = Point(4,-4,3)
        self.assertEqual(p,Tuple(4,-4,3,1))

    def test_vector_creates(self):
        p = Vector(4,-4,3)
        self.assertEqual(p,Tuple(4,-4,3,0))

    def test_add_tuples(self):
        a1 = Tuple(3,-2,5,1)
        a2 = Tuple(-2,3,1,0)
        self.assertEqual(a1+a2, Tuple(1,1,6,1))

    def test_sub_points(self):
        p1 = Point(3,2,1)
        p2 = Point(5,6,7)
        self.assertEqual(p1-p2, Vector(-2,-4,-6))

    def test_sub_point_vector(self):
        p = Point(3,2,1)
        v = Vector(5,6,7)
        self.assertEqual(p-v, Point(-2,-4,-6))

    def test_sub_vector(self):
        v1 = Vector(3,2,1)
        v2 = Vector(5,6,7)
        self.assertEqual(v1-v2, Vector(-2,-4,-6))

    def test_sub_zero(self):
        zero = Vector(0,0,0)
        v = Vector(1,-2,3)
        self.assertEqual(zero-v, Vector(-1,2,-3))

    def test_neg_tuple(self):
        a = Tuple(1,-2,3,-4)
        self.assertEqual(-a,Tuple(-1,2,-3,4))

    def test_scale(self):
        a = Tuple(1,-2,3,-4)
        self.assertEqual(a*3.5,Tuple(3.5,-7,10.5,-14))
        self.assertEqual(a*0.5,Tuple(0.5,-1,1.5,-2))
        self.assertEqual(a/2,Tuple(0.5,-1,1.5,-2))

    def test_magnitude(self):
        v = Vector(1,0,0)
        self.assertEqual(v.magnitude(),1)

        v = Vector(0,1,0)
        self.assertEqual(v.magnitude(),1)

        v = Vector(0,0,1)
        self.assertEqual(v.magnitude(),1)

        v = Vector(1,2,3)
        self.assertEqual(v.magnitude(),math.sqrt(14))

        v = Vector(-1,-2,-3)
        self.assertEqual(v.magnitude(),math.sqrt(14))

    def test_normalize(self):
        v = Vector(4,0,0)
        self.assertEqual(v.normalize(), Vector(1,0,0))

        v = Vector(1,2,3)
        self.assertEqual(v.normalize(), Vector(
            1/math.sqrt(14),2/math.sqrt(14),3/math.sqrt(14)
        ))
        self.assertEqual(v.normalize().magnitude(), 1)

    def test_dot(self):
        a = Vector(1,2,3)
        b = Vector(2,3,4)
        self.assertEqual(a.dot(b), 20)

    def test_cross(self):
        a = Vector(1,2,3)
        b = Vector(2,3,4)
        self.assertEqual(a.cross(b), Vector(-1,2,-1))
        self.assertEqual(b.cross(a), Vector(1,-2,1))

    def test_reflect_45(self):
        v = Vector(1, -1, 0)
        n = Vector(0, 1, 0)
        r = v.reflect(n)
        self.assertEqual(r, Vector(1,1,0))

    def test_reflect_slanted(self):
        v = Vector(0, -1, 0)
        n = Vector(math.sqrt(2)/2, math.sqrt(2)/2, 0)
        r = v.reflect(n)
        self.assertEqual(r, Vector(1,0,0))


def demo_tuples(*args):
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

    p = Projectile(Point(0,1,0), Vector(1,1,0).normalize())
    e = Environment(Vector(0,-0.1,0), Vector(-0.01,0,0))

    while p.position.y > 0:
        print_prj(p)
        p = tick(e,p)

    print("Hit!")
    print_prj(p)
