import sys
import matplotlib.pyplot as plt 
import numpy as np 

f = open(sys.argv[1], 'r')
x = []
y = []
for l in f:
    coords = l.split(',')
    x.append(float(coords[0][1:]))
    y.append(float(coords[1][:-2]))

xpoints = np.array(x)
ypoints = np.array(y)

plt.plot(xpoints, ypoints, 'o')
plt.show()
