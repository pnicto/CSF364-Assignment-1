import sys
from collections import defaultdict


def readPoints(filename):
    points = []
    with open(filename, "r") as f:
        for line in f:
            coords = line.split(",")
            x = coords[0][1:]
            y = coords[1][:-2]
            points.append((int(x), int(y)))
    return points


def compare(a, b):
    if len(a) != len(b):
        return False
    d = defaultdict(int)
    for p in a:
        d[p] += 1

    for p in b:
        d[p] -= 1

    for i in d:
        if d[i] != 0:
            return False
    return True


a = readPoints(sys.argv[1])
b = readPoints(sys.argv[2])

if compare(a, b):
    exit(0)
exit(1)
