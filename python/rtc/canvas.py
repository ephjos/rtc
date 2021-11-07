

from rtc.color import Color

class Canvas:
    def __init__(self, w, h):
        self.w = w
        self.h = h
        self.p = [Color(0,0,0) for _ in range(w*h)]

    def write(self, x, y, color):
        self.p[x + (y * self.w)] = color

    def read(self, x, y):
        return self.p[x + (y * self.w)]

    def ppm(self):
        lines = ["P3", "{} {}".format(self.w, self.h), "255", ""]

        l = self.w*self.h
        s = 5
        for i in range(0, l, s):
            line = ' '.join(map(str, self.p[i:min(i+s, l)]))
            lines.append(line)

        return "\n".join(lines) + "\n"

    def save(self, filename):
        with open(filename, "w") as savefile:
            savefile.write("P3\n{} {}\n255\n\n".format(self.w, self.h))
            l = self.w*self.h
            s = 50
            for i in range(0, l, s):
                line = ' '.join(map(str, self.p[i:min(i+s, l)]))
                savefile.write(line)
                savefile.write("\n")

            savefile.write("\n")
        print("Saved to ", filename)


