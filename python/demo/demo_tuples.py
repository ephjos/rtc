from rtc.tuples import *


def demo_tuples(*args):
    class Projectile:
        def __init__(self, position, velocity):
            self.position = position
            self.velocity = velocity

    class Environment:
        def __init__(self, gravity, wind):
            self.gravity = gravity
            self.wind = wind

    def tick(env, prj):
        position = tuple4_add(prj.position, prj.velocity)
        velocity = tuple4_add(prj.velocity, tuple4_add(env.gravity, env.wind))
        return Projectile(position, velocity)

    def print_prj(p):
        print("{:6.2f} {:6.2f}".format(p.position[0], p.position[1]))

    p = Projectile(Point(0, 1, 0), tuple4_normalize(Vector(1, 1, 0)))
    e = Environment(Vector(0, -0.1, 0), Vector(-0.01, 0, 0))

    while p.position[1] > 0:
        print_prj(p)
        p = tick(e, p)

    print("Hit!")
    print_prj(p)
