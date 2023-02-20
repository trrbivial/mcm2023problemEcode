import numpy as np
import matplotlib.pyplot as plt
with open('lightmap.out') as f:
    n = [int(x) for x in next(f).split()][0]
    arr = []
    for line in f:
        arr.append([float(x) for x in line.split()])
        if len(arr) == n:
            break

with open('terrain.in') as f:
    n = [int(x) for x in next(f).split()][0]
    ty = []
    for line in f:
        ty.append([int(x) for x in line.split()])
        if len(arr) == n:
            break

ROAD = 10
RIVER = 20
PLAIN = 30
FOREST = 40
BUILDING = 50
HOUSE = 60
MOUNTAIN = 70

for i in range(n):
    for j in range(n):

