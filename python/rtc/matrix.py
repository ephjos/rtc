import copy

from rtc.tuples import Tuple4
from rtc.utils import req

from typing import Union


class Matrix:
    def __init__(self, rows: list[list[float]]):
        self.rows = rows

    def __getitem__(self, key: int) -> list[float]:
        return self.rows[key]

    def __str__(self):
        return str(self.rows)

    def __repr__(self):
        return str(self)

    @property
    def shape(self) -> tuple[int, int]:
        if len(self.rows) == 0:
            return (0, 0)
        return (len(self.rows), len(self.rows[0]))

    def __eq__(self, other):
        if not isinstance(other, Matrix):
            raise NotImplementedError()

        shape = self.shape
        if shape != other.shape:
            return False

        for i in range(shape[0]):
            for j in range(shape[1]):
                if not req(self[i][j], other[i][j]):
                    return False

        return True

    def __matmul_matrix__(self, other) -> "Matrix":
        if not isinstance(other, Matrix):
            raise NotImplementedError()
        [p, q] = self.shape
        [r, s] = other.shape

        if q != r:
            raise Exception("Shape mismatch: " + str([p, q]) + " " + str([r, s]))

        M = Matrix([[0 for j in range(s)] for i in range(r)])

        for i in range(p):
            for j in range(s):
                M[i][j] = 0
                for k in range(r):
                    M[i][j] += self[i][k] * other[k][j]

        return M

    def __matmul_tuple__(self, other) -> Tuple4:
        if not isinstance(other, Tuple4):
            raise NotImplementedError()
        result = self.__matmul_matrix__(
            Matrix([[other.x], [other.y], [other.z], [other.w]])
        )
        return Tuple4(result[0][0], result[1][0], result[2][0], result[3][0])

    def __matmul__(self, other) -> Union["Matrix", Tuple4]:
        if isinstance(other, Matrix):
            return self.__matmul_matrix__(other)
        elif isinstance(other, Tuple4):
            return self.__matmul_tuple__(other)
        else:
            raise NotImplementedError()

    @property
    def T(self) -> "Matrix":
        return Matrix(list(map(list, zip(*self.rows))))

    def determinant(self) -> float:
        [p, q] = self.shape
        if p != q:
            raise Exception("Cannot get determinant of non-square matrix")

        if p == 2:
            a = self.rows[0][0]
            b = self.rows[0][1]
            c = self.rows[1][0]
            d = self.rows[1][1]
            return (a * d) - (b * c)

        result = 0.0
        for i in range(p):
            result += self[0][i] * self.cofactor(0, i)
        return result

    def submatrix(self, y: int, x: int) -> "Matrix":
        [p, q] = self.shape
        if p != q:
            raise Exception("Cannot get determinant of non-square matrix")

        rows = copy.deepcopy(self.rows)
        del rows[y]
        for row in rows:
            del row[x]

        return Matrix(rows)

    def minor(self, y: int, x: int) -> float:
        return self.submatrix(y, x).determinant()

    def cofactor(self, y: int, x: int) -> float:
        m = self.minor(y, x)
        if (x + y) % 2 != 0:
            return -1 * m
        return m

    def is_invertible(self) -> bool:
        return self.determinant() != 0

    def inverse(self) -> "Matrix":
        [p, q] = self.shape
        if p != q:
            raise Exception("Cannot get determinant of non-square matrix")

        det = self.determinant()
        if det == 0:
            raise Exception("Determinant is 0, cannot get inverse")
        cofactors = Matrix(
            [[self.cofactor(i, j) / det for j in range(p)] for i in range(p)]
        )
        return cofactors.T


def IdentityMatrix(d: int = 4) -> Matrix:
    return Matrix([[1 if i == j else 0 for j in range(d)] for i in range(d)])
