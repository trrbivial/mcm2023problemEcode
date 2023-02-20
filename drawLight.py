import numpy as np
import matplotlib.pyplot as plt
with open('lightmap.out') as f:
    n = [int(x) for x in next(f).split()][0]
    arr = []
    for line in f:
        arr.append([float(x) for x in line.split()])
        if len(arr) == n:
            break

assert (n % 2 == 1)
x = np.arange(-(n - 1) // 2, (n - 1) // 2 + 1, 1)
y = np.arange(-(n - 1) // 2, (n - 1) // 2 + 1, 1)

X, Y = np.meshgrid(x, y)

Z = np.array(arr)
print(Z.shape)
cset = plt.contourf(X, Y, Z, vmax=150)
# contour = plt.contour(X, Y, Z, 8, colors='k')
# plt.clabel(contour, fontsize=10, colors='k')
plt.xticks(())
plt.yticks(())
plt.colorbar(cset)
plt.show()
