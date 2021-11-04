#!/usr/bin/env python
import math
import unittest

from canvas import Canvas
from color import Color
from matrix import Matrix
from tuples import Point, Vector, Tuple
from utils import req, clamp

class Transform(Matrix):
    @staticmethod
    def identity(d=4):
        return Transform([[1 if i == j else 0 for j in range(d)] for i in range(d)])

    def inverse(self):
        temp = Matrix.inverse(self)
        self = temp
        self.__class__ = Transform
        return self

    def apply(self, temp):
        self = temp @ self
        self.__class__ = Transform
        return self

    def translation(self, x, y, z):
        temp = Matrix([[1, 0, 0, x],
                       [0, 1, 0, y],
                       [0, 0, 1, z],
                       [0, 0, 0, 1]])
        return self.apply(temp)

    def scaling(self, x, y, z):
        temp = Matrix([[x, 0, 0, 0],
                       [0, y, 0, 0],
                       [0, 0, z, 0],
                       [0, 0, 0, 1]])
        return self.apply(temp)

    def rotation_x(self, r):
        temp = Matrix([[1, 0,           0,            0],
                       [0, math.cos(r), -math.sin(r), 0],
                       [0, math.sin(r), math.cos(r),  0],
                       [0, 0,           0,            1]])
        return self.apply(temp)

    def rotation_y(self, r):
        temp = Matrix([[math.cos(r),  0, math.sin(r), 0],
                       [0,            1, 0,           0],
                       [-math.sin(r), 0, math.cos(r), 0],
                       [0,            0, 0,           1]])
        return self.apply(temp)

    def rotation_z(self, r):
        temp = Matrix([[math.cos(r), -math.sin(r), 0, 0],
                       [math.sin(r), math.cos(r),  0, 0],
                       [0,           0,            1, 0],
                       [0,           0,            0, 1]])
        return self.apply(temp)

    def shearing(self, xy, xz, yx, yz, zx, zy):
        temp = Matrix([[1,  xy, xz, 0],
                       [yx, 1,  yz, 0],
                       [zx, zy, 1,  0],
                       [0,  0,  0,  1]])
        return self.apply(temp)

class TestTransform(unittest.TestCase):
    def test_translation(self):
        transform = Transform.identity().translation(5, -3, 2)
        p = Point(-3, 4, 5)
        self.assertEqual(transform @ p, Point(2,1,7))

    def test_translation_inverse(self):
        transform = Transform.identity().translation(5, -3, 2).inverse()
        p = Point(-3, 4, 5)
        self.assertEqual(transform @ p, Point(-8, 7, 3))

    def test_translation_vector(self):
        transform = Transform.identity().translation(5, -3, 2)
        v = Vector(-3, 4, 5)
        self.assertEqual(transform @ v, v)

    def test_scaling_point(self):
        transform = Transform.identity().scaling(2, 3, 4)
        p = Point(-4, 6, 8)
        self.assertEqual(transform @ p, Point(-8, 18, 32))

    def test_scaling_vector(self):
        transform = Transform.identity().scaling(2, 3, 4)
        p = Vector(-4, 6, 8)
        self.assertEqual(transform @ p, Vector(-8, 18, 32))

    def test_scaling_inverse(self):
        transform = Transform.identity().scaling(2, 3, 4).inverse()
        p = Vector(-4, 6, 8)
        self.assertEqual(transform @ p, Vector(-2, 2, 2))

    def test_scaling_reflection(self):
        transform = Transform.identity().scaling(-1, 1, 1).inverse()
        p = Point(2, 3, 4)
        self.assertEqual(transform @ p, Point(-2, 3, 4))

    def test_rotation_x(self):
        p = Point(0, 1, 0)
        half_quarter = Transform.identity().rotation_x(math.pi / 4)
        full_quarter = Transform.identity().rotation_x(math.pi / 2)

        self.assertEqual(half_quarter @ p, Point(0, math.sqrt(2)/2, math.sqrt(2)/2))
        self.assertEqual(full_quarter @ p, Point(0, 0, 1))
        self.assertEqual(half_quarter.inverse() @ p, Point(0, math.sqrt(2)/2, -math.sqrt(2)/2))

    def test_rotation_y(self):
        p = Point(0, 0, 1)
        half_quarter = Transform.identity().rotation_y(math.pi / 4)
        full_quarter = Transform.identity().rotation_y(math.pi / 2)

        self.assertEqual(half_quarter @ p, Point(math.sqrt(2)/2, 0, math.sqrt(2)/2))
        self.assertEqual(full_quarter @ p, Point(1, 0, 0))

    def test_rotation_z(self):
        p = Point(0, 1, 0)
        half_quarter = Transform.identity().rotation_z(math.pi / 4)
        full_quarter = Transform.identity().rotation_z(math.pi / 2)

        self.assertEqual(half_quarter @ p, Point(-math.sqrt(2)/2, math.sqrt(2)/2, 0))
        self.assertEqual(full_quarter @ p, Point(-1, 0, 0))

    def test_shearing(self):
        transform = Transform.identity().shearing(1, 0, 0, 0, 0, 0)
        p = Point(2,3,4)
        self.assertEqual(transform @ p, Point(5, 3, 4))

        transform = Transform.identity().shearing(0, 1, 0, 0, 0, 0)
        self.assertEqual(transform @ p, Point(6, 3, 4))

        transform = Transform.identity().shearing(0, 0, 1, 0, 0, 0)
        self.assertEqual(transform @ p, Point(2, 5, 4))

        transform = Transform.identity().shearing(0, 0, 0, 1, 0, 0)
        self.assertEqual(transform @ p, Point(2, 7, 4))

        transform = Transform.identity().shearing(0, 0, 0, 0, 1, 0)
        self.assertEqual(transform @ p, Point(2, 3, 6))

        transform = Transform.identity().shearing(0, 0, 0, 0, 0, 1)
        self.assertEqual(transform @ p, Point(2, 3, 7))

    def test_individual_chaining(self):
        p = Point(1, 0, 1)
        A = Transform.identity().rotation_x(math.pi/2)
        B = Transform.identity().scaling(5, 5, 5)
        C = Transform.identity().translation(10, 5, 7)

        p2 = A @ p
        self.assertEqual(p2, Point(1, -1, 0))

        p3 = B @ p2
        self.assertEqual(p3, Point(5, -5, 0))

        p4 = C @ p3
        self.assertEqual(p4, Point(15, 0, 7))

    def test_group_chaining(self):
        p = Point(1, 0, 1)
        A = Transform.identity().rotation_x(math.pi/2)
        B = Transform.identity().scaling(5, 5, 5)
        C = Transform.identity().translation(10, 5, 7)
        T = C @ B @ A

        self.assertEqual(T @ p, Point(15, 0, 7))

        # pylint: disable=no-member
        T2 = Transform.identity().rotation_x(math.pi/2).scaling(5, 5, 5).translation(10, 5, 7)
        self.assertEqual(T, T2)
        self.assertEqual(T2 @ p, Point(15, 0, 7))

def demo_transform(*args):
    size = 50
    canvas = Canvas(size, size)
    tick = Color(1,1,1)

    for i in range(12):
        p = Point(1, 0, 0)
        r = i * (math.pi/6)
        T = Transform \
                .identity() \
                .rotation_z(r) \
                .scaling(size/4, size/4, size/4) \
                .translation(size/2, size/2, 0)
        Tp = T @ p
        x = round(Tp.x)
        y = round(Tp.y)
        canvas.write(x, y, tick)

    canvas.save("./transform_demo.ppm")

