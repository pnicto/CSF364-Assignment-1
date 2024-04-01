from scipy.spatial import ConvexHull
points = []
inFile = open("in.txt", "r")
for x in inFile:
    coords = x.split(',')
    points.append(((float(coords[0][1:])), float(coords[1][:-2])))

hull = ConvexHull(points)
outFile = open("pyout.txt", "w")
for i in hull.vertices:
    outFile.write(f'({int(points[i][0])},{int(points[i][1])})\n')
