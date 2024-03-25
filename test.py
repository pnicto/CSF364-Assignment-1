from scipy.spatial import ConvexHull
points = []
inFile = open("in.txt", "r")
for x in inFile:
    coords = x.split(',')
    points.append(((int(coords[0][1:])), int(coords[1][:-2])))

hull = ConvexHull(points)
outFile = open("pyout.txt", "w")
for i in hull.vertices:
    print(f'({points[i][0]},{points[i][1]})')
    outFile.write(f'({points[i][0]},{points[i][1]})\n')
