import sys
f = open(sys.argv[1], 'r')
for line in f:
    times = line.split(' ')
    sum = 0
    if len(times) > 2:
        for time in times[1:-1]:
            sum += float(time)
    print(f"The average time for {times[0]} points is { round(sum / 10, 3) } seconds.")

