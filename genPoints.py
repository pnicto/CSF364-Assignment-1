from shapely.geometry import Point, Polygon
import random, sys, math

def generate_random_points(num_points, min_value, max_value):
    points = []
    for _ in range(num_points):
        x = random.uniform(min_value, max_value)
        x = int(x)
        y = random.uniform(min_value, max_value)
        y = int(y)
        points.append((x, y))
    return points

def drawPolygon(vertex_points, num_points, radius):
    angle = 0
    angle_increment = 360 / vertex_points 
    points = []
    for _ in range(vertex_points):
        x = radius * math.cos(math.radians(angle))
        y = radius * math.sin(math.radians(angle))
        points.append((int(x), int(y)))
        angle += angle_increment
    polygon = Polygon(points)
    i = 0
    while i < num_points:
        x = random.uniform(-1*radius, radius)
        x = int(x)
        y = random.uniform(-1*radius, radius)
        y = int(y)
        if polygon.contains_properly(Point(x, y)):
            points.append((x, y))
            i += 1
    return points

flag = sys.argv[1]
num_points = 0
if flag != "--uniform" and flag != "--polygon":
    print("Invalid flag. Use --uniform or --polygon")
    sys.exit(1)
else:
    num_points = int(sys.argv[2])
    min_value = -10 * num_points
    max_value = 10 * num_points
    vertex_points = num_points
    if flag == "--uniform":
        random_points = generate_random_points(num_points, min_value, max_value)
    else:
        random_points = drawPolygon(vertex_points, num_points, num_points)

outFile = open("in.txt", "w")
for p in random_points:
    outFile.write(f'({p[0]},{p[1]})\n')
