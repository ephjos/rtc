import copy

from rtc.tuples import Tuple
from rtc.utils import req


class Matrix:
    def __init__(self, rows):
        self.rows = rows

    def __getitem__(self, key):
        return self.rows[key]

    def __str__(self):
        return str(self.rows)

    def __repr__(self):
        return str(self)

    @property
    def shape(self):
        if len(self.rows) == 0:
            return [0, 0]
        return [len(self.rows), len(self.rows[0])]

    def __eq__(self, o):
        shape = self.shape
        if shape != o.shape:
            return False

        for i in range(shape[0]):
            for j in range(shape[1]):
                if not req(self[i][j], o[i][j]):
                    return False

        return True

    def __matmul_matrix__(self, o):
        [p, q] = self.shape
        [r, s] = o.shape

        if q != r:
            raise Exception("Shape mismatch: " + str([p, q]) + " " + str([r, s]))

        M = Matrix([[0 for j in range(s)] for i in range(r)])

        for i in range(p):
            for j in range(s):
                M[i][j] = 0
                for k in range(r):
                    M[i][j] += self[i][k] * o[k][j]

        return M

    def __matmul_tuple__(self, o):
        result = self.__matmul_matrix__(Matrix([[o.x], [o.y], [o.z], [o.w]]))
        return Tuple(result[0][0], result[1][0], result[2][0], result[3][0])

    def __matmul__(self, o):
        if isinstance(o, Matrix):
            return self.__matmul_matrix__(o)
        else:
            return self.__matmul_tuple__(o)

    @property
    def T(self):
        return Matrix(list(map(list, zip(*self.rows))))

    def determinant(self):
        [p, q] = self.shape
        if p != q:
            raise Exception("Cannot get determinant of non-square matrix")

        if p == 2:
            a = self.rows[0][0]
            b = self.rows[0][1]
            c = self.rows[1][0]
            d = self.rows[1][1]
            return (a * d) - (b * c)

        result = 0
        for i in range(p):
            result += self[0][i] * self.cofactor(0, i)
        return result

    def submatrix(self, y, x):
        [p, q] = self.shape
        if p != q:
            raise Exception("Cannot get determinant of non-square matrix")

        rows = copy.deepcopy(self.rows)
        del rows[y]
        for row in rows:
            del row[x]

        return Matrix(rows)

    def minor(self, y, x):
        return self.submatrix(y, x).determinant()

    def cofactor(self, y, x):
        m = self.minor(y, x)
        if (x + y) % 2 != 0:
            return -1 * m
        return m

    def is_invertible(self):
        return self.determinant() != 0

    def inverse(self):
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


def IdentityMatrix(d=4):
    return Matrix([[1 if i == j else 0 for j in range(d)] for i in range(d)])
