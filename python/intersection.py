#!/usr/bin/env python

import math
import unittest

class Intersection:
    def __init__(self, t, o):
        self.t = t
        self.object = o

class Intersections:
    def __init__(self, *args):
        self.intersections = [*args]

    def __getitem__(self, index):
        return self.intersections[index]

    def __len__(self):
        return len(self.intersections)

    def hit(self):
        if len(self) == 0:
            return None

        curr = self.intersections[0]
        for i in self.intersections[1:]:
            if i.t > 0 and i.t < curr.t:
                curr = i

        if curr.t < 0:
            return None
        return curr
