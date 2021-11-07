

# Rough equals
def req(a,b):
    return abs(a-b) < 0.00001

def clamp(x, a, b):
    if x < a:
        return a

    if x > b:
        return b

    return x
