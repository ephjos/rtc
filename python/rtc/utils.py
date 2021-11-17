EPSILON = 0.0001


# Rough equals
def req(a, b):
    return abs(a - b) < EPSILON


def clamp(x, a, b):
    if x < a:
        return a

    if x > b:
        return b

    return x
