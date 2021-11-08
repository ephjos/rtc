from rtc.tuples import Point, Vector


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
        position = prj.position + prj.velocity
        velocity = prj.velocity + env.gravity + env.wind
        return Projectile(position, velocity)

    def print_prj(p):
        print("{:6.2f} {:6.2f}".format(p.position.x, p.position.y))

    p = Projectile(Point(0, 1, 0), Vector(1, 1, 0).normalize())
    e = Environment(Vector(0, -0.1, 0), Vector(-0.01, 0, 0))

    while p.position.y > 0:
        print_prj(p)
        p = tick(e, p)

    print("Hit!")
    print_prj(p)
