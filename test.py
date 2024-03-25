from scipy.spatial import ConvexHull
import numpy as np
rng = np.random.default_rng()
points = []
f = open("in.txt", "r")
for x in f:
    coords = x.split(',')
    points.append(((int(coords[0][1:])), int(coords[1][:-2])))

hull = ConvexHull(points)
for i in hull.vertices:
    print(points[i])
