import numpy as np
from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt

fig = plt.figure(figsize=(12, 10))
ax = plt.axes(projection='3d')

x = np.arange(-1, 1, 0.02)
y = np.arange(-1, 1, 0.02)

X, Y = np.meshgrid(x, y)

pi = 3.1415926
sig = 1 / 6

Z = np.exp(-0.5 * ((X / sig) * (X / sig) + (Y / sig) *
                   (Y / sig))) / (2 * pi * sig * sig)

surf = ax.plot_surface(X, Y, Z, cmap=plt.cm.cividis)

# Set axes label
ax.set_xlabel('x', labelpad=20)
ax.set_ylabel('y', labelpad=20)
ax.set_zlabel('z', labelpad=20)

fig.colorbar(surf, shrink=0.5, aspect=8)

plt.show()
