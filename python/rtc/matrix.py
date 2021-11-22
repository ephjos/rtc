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
        a = self
        b = other
        [p, q] = a.shape
        [r, s] = b.shape

        if q != r:
            raise Exception("Shape mismatch: " + str([p, q]) + " " + str([r, s]))

        aa = self[0][0]
        ab = self[0][1]
        ac = self[0][2]
        ad = self[0][3]
        ae = self[1][0]
        af = self[1][1]
        ag = self[1][2]
        ah = self[1][3]
        ai = self[2][0]
        aj = self[2][1]
        ak = self[2][2]
        al = self[2][3]
        am = self[3][0]
        an = self[3][1]
        ao = self[3][2]
        ap = self[3][3]

        ba = other[0][0]
        bb = other[0][1]
        bc = other[0][2]
        bd = other[0][3]
        be = other[1][0]
        bf = other[1][1]
        bg = other[1][2]
        bh = other[1][3]
        bi = other[2][0]
        bj = other[2][1]
        bk = other[2][2]
        bl = other[2][3]
        bm = other[3][0]
        bn = other[3][1]
        bo = other[3][2]
        bp = other[3][3]

        rows = [[aa*ba+ab*be+ac*bi+ad*bm,
                 aa*bb+ab*bf+ac*bj+ad*bn,
                 aa*bc+ab*bg+ac*bk+ad*bo,
                 aa*bd+ab*bh+ac*bl+ad*bp],
                [ae*ba+af*be+ag*bi+ah*bm,
                 ae*bb+af*bf+ag*bj+ah*bn,
                 ae*bc+af*bg+ag*bk+ah*bo,
                 ae*bd+af*bh+ag*bl+ah*bp],
                [ai*ba+aj*be+ak*bi+al*bm,
                 ai*bb+aj*bf+ak*bj+al*bn,
                 ai*bc+aj*bg+ak*bk+al*bo,
                 ai*bd+aj*bh+ak*bl+al*bp],
                [am*ba+an*be+ao*bi+ap*bm,
                 am*bb+an*bf+ao*bj+ap*bn,
                 am*bc+an*bg+ao*bk+ap*bo,
                 am*bd+an*bh+ao*bl+ap*bp]]

        return Matrix(rows)

    def __matmul_tuple__(self, other) -> Tuple4:
        aa = self[0][0]
        ab = self[0][1]
        ac = self[0][2]
        ad = self[0][3]
        ae = self[1][0]
        af = self[1][1]
        ag = self[1][2]
        ah = self[1][3]
        ai = self[2][0]
        aj = self[2][1]
        ak = self[2][2]
        al = self[2][3]
        am = self[3][0]
        an = self[3][1]
        ao = self[3][2]
        ap = self[3][3]

        ba = other[0]
        bb = other[1]
        bc = other[2]
        bd = other[3]

        return Tuple4(aa*ba+ab*bb+ac*bc+ad*bd,
                      ae*ba+af*bb+ag*bc+ah*bd,
                      ai*ba+aj*bb+ak*bc+al*bd,
                      am*ba+an*bb+ao*bc+ap*bd)

    def __matmul__(self, other) -> Union["Matrix", Tuple4]:
        if isinstance(other, Matrix):
            return self.__matmul_matrix__(other)
        return self.__matmul_tuple__(other)

    @property
    def T(self) -> "Matrix":
        return Matrix(list(map(list, zip(*self.rows))))

    def determinant(self) -> float:
        [p, q] = self.shape
        if p != q:
            raise Exception("Cannot get determinant of non-square matrix")

        a = self[0][0]
        b = self[0][1]
        c = self[0][2]
        d = self[0][3]
        e = self[1][0]
        f = self[1][1]
        g = self[1][2]
        h = self[1][3]
        i = self[2][0]
        j = self[2][1]
        k = self[2][2]
        l = self[2][3]
        m = self[3][0]
        n = self[3][1]
        o = self[3][2]
        p = self[3][3]

        ca = f * (k*p - l*o) - g * (j*p - l*n) + h * (j*o - k*n)
        cb = -e * (k*p - l*o) + g * (i*p - l*m) - h * (i*o - k*m)
        cc = e * (j*p - l*n) - f * (i*p - l*m) + h * (i*n - j*m)
        cd = -e * (j*o - k*n) + f * (i*o - k*m) - g * (i*n - j*m)

        return a * ca + b * cb + c * cc + d * cd

    def is_invertible(self) -> bool:
        return self.determinant() != 0

    def inverse(self) -> "Matrix":
        [p, q] = self.shape
        if p != q:
            raise Exception("Cannot get determinant of non-square matrix")

        a = self[0][0]
        b = self[0][1]
        c = self[0][2]
        d = self[0][3]
        e = self[1][0]
        f = self[1][1]
        g = self[1][2]
        h = self[1][3]
        i = self[2][0]
        j = self[2][1]
        k = self[2][2]
        l = self[2][3]
        m = self[3][0]
        n = self[3][1]
        o = self[3][2]
        p = self[3][3]

        ca = f * (k*p - l*o) - g * (j*p - l*n) + h * (j*o - k*n)
        cb = -e * (k*p - l*o) + g * (i*p - l*m) - h * (i*o - k*m)
        cc = e * (j*p - l*n) - f * (i*p - l*m) + h * (i*n - j*m)
        cd = -e * (j*o - k*n) + f * (i*o - k*m) - g * (i*n - j*m)

        det = a * ca + b * cb + c * cc + d * cd
        if det == 0:
            raise Exception("Determinant is 0, cannot get inverse")

        ce = -b * (k*p - l*o) + c * (j*p - l*n) - d * (j*o - k*n)
        cf = a * (k*p - l*o) - c * (i*p - l*m) + d * (i*o - k*m)
        cg = -a * (j*p - l*n) + b * (i*p - l*m) - d * (i*n - j*m)
        ch = a * (j*o - k*n) - b * (i*o - k*m) + c * (i*n - j*m)

        ci = b * (g*p - h*o) - c * (f*p - h*n) + d * (f*o - g*n)
        cj = -a * (g*p - h*o) + c * (e*p - h*m) - d * (e*o - g*m)
        ck = a * (f*p - h*n) - b * (e*p - h*m) + d * (e*n - f*m)
        cl = -a * (f*o - g*n) + b * (e*o - g*m) -c * (e*n - f*m)

        cm = -b * (g*l - h*k) + c * (f*l - h*j) -d * (f*k - g*j)
        cn = a * (g*l - h*k) - c * (e*l - h*i) + d * (e*k - g*i)
        co = -a * (f*l - h*j) + b * (e*l - h*i) - d * (e*j - f*i)
        cp = a * (f*k - g*j) - b * (e*k - g*i) + c * (e*j - f*i)


        return Matrix([[ca/det, ce/det, ci/det, cm/det],
                       [cb/det, cf/det, cj/det, cn/det],
                       [cc/det, cg/det, ck/det, co/det],
                       [cd/det, ch/det, cl/det, cp/det]])


def IdentityMatrix(d: int = 4) -> Matrix:
    return Matrix([[1 if i == j else 0 for j in range(d)] for i in range(d)])
