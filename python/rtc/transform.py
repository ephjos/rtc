import math

from rtc.matrix import Matrix


class Transform(Matrix):
    def __init__(self, d=4):
        Matrix.__init__(
            self, [[1 if i == j else 0 for j in range(d)] for i in range(d)]
        )
        self.__class__ = Transform

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
        temp = Matrix([[1, 0, 0, x], [0, 1, 0, y], [0, 0, 1, z], [0, 0, 0, 1]])
        return self.apply(temp)

    def scaling(self, x, y, z):
        temp = Matrix([[x, 0, 0, 0], [0, y, 0, 0], [0, 0, z, 0], [0, 0, 0, 1]])
        return self.apply(temp)

    def rotation_x(self, r):
        temp = Matrix(
            [
                [1, 0, 0, 0],
                [0, math.cos(r), -math.sin(r), 0],
                [0, math.sin(r), math.cos(r), 0],
                [0, 0, 0, 1],
            ]
        )
        return self.apply(temp)

    def rotation_y(self, r):
        temp = Matrix(
            [
                [math.cos(r), 0, math.sin(r), 0],
                [0, 1, 0, 0],
                [-math.sin(r), 0, math.cos(r), 0],
                [0, 0, 0, 1],
            ]
        )
        return self.apply(temp)

    def rotation_z(self, r):
        temp = Matrix(
            [
                [math.cos(r), -math.sin(r), 0, 0],
                [math.sin(r), math.cos(r), 0, 0],
                [0, 0, 1, 0],
                [0, 0, 0, 1],
            ]
        )
        return self.apply(temp)

    def shearing(self, xy, xz, yx, yz, zx, zy):
        temp = Matrix([[1, xy, xz, 0], [yx, 1, yz, 0], [zx, zy, 1, 0], [0, 0, 0, 1]])
        return self.apply(temp)


def ViewTransform(at, to, up):
    forward = (to - at).normalize()
    upn = up.normalize()
    left = forward.cross(upn)
    true_up = left.cross(forward)

    orientation = Matrix(
        [
            [left.x, left.y, left.z, 0],
            [true_up.x, true_up.y, true_up.z, 0],
            [-forward.x, -forward.y, -forward.z, 0],
            [0, 0, 0, 1],
        ]
    )

    return orientation @ Transform().translation(-at.x, -at.y, -at.z)
