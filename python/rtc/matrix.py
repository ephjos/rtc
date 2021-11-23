from rtc.tuples import Tuple4
from rtc.utils import req

from typing import Union


class Matrix:
    def __init__(self, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p):
        self.a = a
        self.b = b
        self.c = c
        self.d = d
        self.e = e
        self.f = f
        self.g = g
        self.h = h
        self.i = i
        self.j = j
        self.k = k
        self.l = l
        self.m = m
        self.n = n
        self.o = o
        self.p = p

    def __eq__(self, other):
        if not isinstance(other, Matrix):
            raise NotImplementedError()

        return (
            req(self.a, other.a)
            and req(self.b, other.b)
            and req(self.c, other.c)
            and req(self.d, other.d)
            and req(self.e, other.e)
            and req(self.f, other.f)
            and req(self.g, other.g)
            and req(self.h, other.h)
            and req(self.i, other.i)
            and req(self.j, other.j)
            and req(self.k, other.k)
            and req(self.l, other.l)
            and req(self.m, other.m)
            and req(self.n, other.n)
            and req(self.o, other.o)
            and req(self.p, other.p)
        )

    def matmul(self, other) -> "Matrix":
        aa = self.a
        ab = self.b
        ac = self.c
        ad = self.d
        ae = self.e
        af = self.f
        ag = self.g
        ah = self.h
        ai = self.i
        aj = self.j
        ak = self.k
        al = self.l
        am = self.m
        an = self.n
        ao = self.o
        ap = self.p

        ba = other.a
        bb = other.b
        bc = other.c
        bd = other.d
        be = other.e
        bf = other.f
        bg = other.g
        bh = other.h
        bi = other.i
        bj = other.j
        bk = other.k
        bl = other.l
        bm = other.m
        bn = other.n
        bo = other.o
        bp = other.p

        return Matrix(
            aa * ba + ab * be + ac * bi + ad * bm,
            aa * bb + ab * bf + ac * bj + ad * bn,
            aa * bc + ab * bg + ac * bk + ad * bo,
            aa * bd + ab * bh + ac * bl + ad * bp,
            ae * ba + af * be + ag * bi + ah * bm,
            ae * bb + af * bf + ag * bj + ah * bn,
            ae * bc + af * bg + ag * bk + ah * bo,
            ae * bd + af * bh + ag * bl + ah * bp,
            ai * ba + aj * be + ak * bi + al * bm,
            ai * bb + aj * bf + ak * bj + al * bn,
            ai * bc + aj * bg + ak * bk + al * bo,
            ai * bd + aj * bh + ak * bl + al * bp,
            am * ba + an * be + ao * bi + ap * bm,
            am * bb + an * bf + ao * bj + ap * bn,
            am * bc + an * bg + ao * bk + ap * bo,
            am * bd + an * bh + ao * bl + ap * bp,
        )

    def matmul_tuple(self, other) -> Tuple4:
        aa = self.a
        ab = self.b
        ac = self.c
        ad = self.d
        ae = self.e
        af = self.f
        ag = self.g
        ah = self.h
        ai = self.i
        aj = self.j
        ak = self.k
        al = self.l
        am = self.m
        an = self.n
        ao = self.o
        ap = self.p

        ba = other[0]
        bb = other[1]
        bc = other[2]
        bd = other[3]

        return Tuple4(
            aa * ba + ab * bb + ac * bc + ad * bd,
            ae * ba + af * bb + ag * bc + ah * bd,
            ai * ba + aj * bb + ak * bc + al * bd,
            am * ba + an * bb + ao * bc + ap * bd,
        )

    @property
    def T(self) -> "Matrix":
        return Matrix(
            self.a,
            self.e,
            self.i,
            self.m,
            self.b,
            self.f,
            self.j,
            self.n,
            self.c,
            self.g,
            self.k,
            self.o,
            self.d,
            self.h,
            self.l,
            self.p,
        )

    def determinant(self) -> float:
        a = self.a
        b = self.b
        c = self.c
        d = self.d
        e = self.e
        f = self.f
        g = self.g
        h = self.h
        i = self.i
        j = self.j
        k = self.k
        l = self.l
        m = self.m
        n = self.n
        o = self.o
        p = self.p

        ca = f * (k * p - l * o) - g * (j * p - l * n) + h * (j * o - k * n)
        cb = -e * (k * p - l * o) + g * (i * p - l * m) - h * (i * o - k * m)
        cc = e * (j * p - l * n) - f * (i * p - l * m) + h * (i * n - j * m)
        cd = -e * (j * o - k * n) + f * (i * o - k * m) - g * (i * n - j * m)

        return a * ca + b * cb + c * cc + d * cd

    def is_invertible(self) -> bool:
        return self.determinant() != 0

    def inverse(self) -> "Matrix":
        a = self.a
        b = self.b
        c = self.c
        d = self.d
        e = self.e
        f = self.f
        g = self.g
        h = self.h
        i = self.i
        j = self.j
        k = self.k
        l = self.l
        m = self.m
        n = self.n
        o = self.o
        p = self.p

        ca = f * (k * p - l * o) - g * (j * p - l * n) + h * (j * o - k * n)
        cb = -e * (k * p - l * o) + g * (i * p - l * m) - h * (i * o - k * m)
        cc = e * (j * p - l * n) - f * (i * p - l * m) + h * (i * n - j * m)
        cd = -e * (j * o - k * n) + f * (i * o - k * m) - g * (i * n - j * m)

        det = a * ca + b * cb + c * cc + d * cd
        if det == 0:
            raise Exception("Determinant is 0, cannot get inverse")

        ce = -b * (k * p - l * o) + c * (j * p - l * n) - d * (j * o - k * n)
        cf = a * (k * p - l * o) - c * (i * p - l * m) + d * (i * o - k * m)
        cg = -a * (j * p - l * n) + b * (i * p - l * m) - d * (i * n - j * m)
        ch = a * (j * o - k * n) - b * (i * o - k * m) + c * (i * n - j * m)

        ci = b * (g * p - h * o) - c * (f * p - h * n) + d * (f * o - g * n)
        cj = -a * (g * p - h * o) + c * (e * p - h * m) - d * (e * o - g * m)
        ck = a * (f * p - h * n) - b * (e * p - h * m) + d * (e * n - f * m)
        cl = -a * (f * o - g * n) + b * (e * o - g * m) - c * (e * n - f * m)

        cm = -b * (g * l - h * k) + c * (f * l - h * j) - d * (f * k - g * j)
        cn = a * (g * l - h * k) - c * (e * l - h * i) + d * (e * k - g * i)
        co = -a * (f * l - h * j) + b * (e * l - h * i) - d * (e * j - f * i)
        cp = a * (f * k - g * j) - b * (e * k - g * i) + c * (e * j - f * i)

        return Matrix(
            ca / det,
            ce / det,
            ci / det,
            cm / det,
            cb / det,
            cf / det,
            cj / det,
            cn / det,
            cc / det,
            cg / det,
            ck / det,
            co / det,
            cd / det,
            ch / det,
            cl / det,
            cp / det,
        )


def IdentityMatrix() -> Matrix:
    return Matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)
