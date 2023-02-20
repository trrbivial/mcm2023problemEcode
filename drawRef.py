import matplotlib.pyplot as plt
import numpy as np

fig = plt.figure(figsize=(12.8, 12.8))
ax = fig.gca()
"""
x = [-6.5, -3.125, 1.375, 4, 2, 6, 4.875, 1.875, -0.375, -3.375, -6.75]
y = [5.5, 1, 7, 3.5, 0.83333333, -4.5, -6, -2, -5, -1, -5.5]
"""
"""
pi = 3.1415926535898
n = 20
x = []
y = []
for i in range(n):
    x.append(0.5)
    y.append(0.5)
    x.append(0.5 + np.cos(2 * pi * i / n))
    y.append(0.5 + np.sin(2 * pi * i / n))
"""

x = [-6.5]
y = [6.5]

for i in range(7):
    y.append(i)
    x.append(-9.0 / 7.0 * (i - 6.5) - 6.5)

for i in range(7):
    x.append(i - 5)
    y.append(6.5 + (-7.0 / 9.0 * (i - 5 + 6.5)))

plt.plot(x, y, marker="*")
ax.set_xticks(np.arange(-10, 10, 1))
ax.set_yticks(np.arange(-10, 10, 1))

#ax.axes.xaxis.set_ticklabels([])
#ax.axes.yaxis.set_ticklabels([])
plt.grid(visible=True, linestyle="-", color='black')
plt.show()
