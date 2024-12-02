
def _m4_transpose():
    print("// START m4_transpose")

    for i in range(4):
        for j in range(4):
            print(f"out[{i} _ {j}] = A[{j} _ {i}];")

    print("// END m4_transpose")
    print()

def _m4_mul():
    print("// START m4_mul")

    for i in range(4):
        print(f"v4 a{i} = {{ A[{i} _ 0], A[{i} _ 1], A[{i} _ 2], A[{i} _ 3] }};")
    print()

    for j in range(4):
        print(f"v4 b{j} = {{ B[0 _ {j}], B[1 _ {j}], B[2 _ {j}], B[3 _ {j}] }};")
    print()

    for i in range(4):
        for j in range(4):
            print(f"out[{i} _ {j}] = v4_dot(a{i}, b{j});")

    print("// END m4_mul")
    print()

def m4_transpose():
    print("// START m4_transpose")

    print(f"const m4 temp = {{")
    for i in range(4):
        s = ""
        for j in range(4):
            #print(f"v4_dot(a{i}, b{j}), ")
            s += f"A[{j} _ {i}], "
        print(f"  {s}")
    print(f"}};")
    print(f"memcpy(out, temp, sizeof(m4));")

    print("// END m4_transpose")
    print()

def m4_mul():
    print("// START m4_mul")

    for i in range(4):
        print(f"v4 a{i} = {{ A[{i} _ 0], A[{i} _ 1], A[{i} _ 2], A[{i} _ 3] }};")
    print()

    for j in range(4):
        print(f"v4 b{j} = {{ B[0 _ {j}], B[1 _ {j}], B[2 _ {j}], B[3 _ {j}] }};")
    print()

    print(f"const m4 temp = {{")
    for i in range(4):
        s = ""
        for j in range(4):
            #print(f"v4_dot(a{i}, b{j}), ")
            s += f"v4_dot(a{i}, b{j}), "
        print(f"  {s}")
    print(f"}};")
    print(f"memcpy(out, temp, sizeof(m4));")

    print("// END m4_mul")
    print()

def main():
    m4_transpose()
    m4_mul()

    return 0

main()
