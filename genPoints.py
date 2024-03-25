import random, sys

def generate_random_points(num_points, min_value, max_value):
    points = []
    for _ in range(num_points):
        x = random.uniform(min_value, max_value)
        y = random.uniform(min_value, max_value)
        points.append((x, y))
    return points

num_points = int(sys.argv[1])
min_value = 0
max_value = 100
random_points = generate_random_points(num_points, min_value, max_value)
outFile = open("in.txt", "w")
for p in random_points:
    outFile.write(f'({p[0]},{p[1]})\n')