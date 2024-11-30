
def _matrix4_transpose():
    print("// START matrix4_transpose")

    for i in range(4):
        for j in range(4):
            print(f"out[{i} _ {j}] = A[{j} _ {i}];")

    print("// END matrix4_transpose")
    print()

def _matrix4_mul():
    print("// START matrix4_mul")

    for i in range(4):
        print(f"vec4 a{i} = {{ A[{i} _ 0], A[{i} _ 1], A[{i} _ 2], A[{i} _ 3] }};")
    print()

    for j in range(4):
        print(f"vec4 b{j} = {{ B[0 _ {j}], B[1 _ {j}], B[2 _ {j}], B[3 _ {j}] }};")
    print()

    for i in range(4):
        for j in range(4):
            print(f"out[{i} _ {j}] = vec4_dot(a{i}, b{j});")

    print("// END matrix4_mul")
    print()

def matrix4_transpose():
    print("// START matrix4_transpose")

    print(f"const matrix4 temp = {{")
    for i in range(4):
        s = ""
        for j in range(4):
            #print(f"vec4_dot(a{i}, b{j}), ")
            s += f"A[{j} _ {i}], "
        print(f"  {s}")
    print(f"}};")
    print(f"memcpy(out, temp, sizeof(matrix4));")

    print("// END matrix4_transpose")
    print()

def matrix4_mul():
    print("// START matrix4_mul")

    for i in range(4):
        print(f"vec4 a{i} = {{ A[{i} _ 0], A[{i} _ 1], A[{i} _ 2], A[{i} _ 3] }};")
    print()

    for j in range(4):
        print(f"vec4 b{j} = {{ B[0 _ {j}], B[1 _ {j}], B[2 _ {j}], B[3 _ {j}] }};")
    print()

    print(f"const matrix4 temp = {{")
    for i in range(4):
        s = ""
        for j in range(4):
            #print(f"vec4_dot(a{i}, b{j}), ")
            s += f"vec4_dot(a{i}, b{j}), "
        print(f"  {s}")
    print(f"}};")
    print(f"memcpy(out, temp, sizeof(matrix4));")

    print("// END matrix4_mul")
    print()

def main():
    matrix4_transpose()
    matrix4_mul()

    return 0

main()
