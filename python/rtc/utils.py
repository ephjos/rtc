EPSILON: float = 0.0001


# Rough equals
def req(a: float, b: float) -> bool:
    return abs(a - b) < EPSILON


def clamp(x: float, a: float, b: float) -> float:
    if x < a:
        return a

    if x > b:
        return b

    return x
