import os

n = 64

os.system("g++ --std=c++17 disturb.cpp -o disturb")
os.system("g++ --std=c++17 spreadLight.cpp -o spreadLight")

x = []
y = []
for i in range(100):
    os.system("./disturb {}".format(1))
    os.system("./spreadLight")
    with open('disturb.txt') as f:
        x.append(i)
        y.append([float(z) for z in next(f).split()][0])

for i in range(len(y)):
    if i > 0:
        y[i] = y[i] / y[0] - 1

y[0] = 0

import numpy as np
import matplotlib.pyplot as plt

plt.plot(x, y)
plt.show()
