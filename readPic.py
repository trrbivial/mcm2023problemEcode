from PIL import Image
import numpy as np
import random


def load_txt(filename: str, dtype: str):
    with open(filename) as f:
        shape = list(map(int, f.readline().split()))
        if dtype.startswith('int'):
            data = list(map(int, f.readline().split()))
        elif dtype.startswith('float'):
            data = list(map(float, f.readline().split()))
        else:
            assert False
        return np.array(data, dtype=dtype).reshape(shape)


def store_txt(filename: str, tensor: np.array):
    with open(filename, "w") as f:
        f.write(" ".join(map(str, tensor.shape)) + "\n")
        f.write(" ".join(map(str, tensor.flatten())) + "\n")


ROAD = 10
RIVER = 20
PLAIN = 30
FOREST = 40
BUILDING = 50
HOUSE = 60
MOUNTAIN = 70

ROAD_T = (0x98, 0x98, 0x98)
RIVER_T = (0, 170, 255)
PLAIN_T = (0x4d, 0xe7, 0)
FOREST_T = (0, 0xaa, 0)
MOUNTAIN_T = (0, 0x55, 0)
BUILDING_T = (0x55, 0x55, 0x7f)
HOUSE_T = (0x6f, 0x6f, 0xa6)


def procBeijing():
    img = Image.open('beijing2.png')
    pix = img.load()
    print(img.size)
    print(pix[0, 0])

    n = img.size[0]

    mp = np.zeros((n, n), dtype='int32')
    for i in range(n):
        for j in range(n):
            if pix[i, j] == (0x4d, 0xe7, 0, 255):
                pix[i, j] = (0, 0xaa, 0, 255)
                mp[i, j] = FOREST
            elif pix[i, j] == (0, 170, 255, 255):
                mp[i, j] = RIVER
                pass
            elif pix[i, j] == (0x98, 0x98, 0x98, 255):
                mp[i, j] = ROAD
                pass
            else:
                mp[i, j] = PLAIN
                pix[i, j] = (0x4d, 0xe7, 0, 255)
    img.save('beijing3.png')
    store_txt('beijing4.in', mp)


def procCountryside():
    img = Image.open('countryside1.png')
    pix = img.load()
    print(img.size)

    n = img.size[0]

    mp = np.zeros((n, n), dtype='int32')
    for i in range(n):
        for j in range(n):
            if pix[i, j] == (0x4d, 0xe7, 0, 255):
                mp[i, j] = PLAIN
            elif pix[i, j] == (0, 170, 255, 255):
                mp[i, j] = RIVER
            elif pix[i, j] == (0x98, 0x98, 0x98, 255):
                mp[i, j] = ROAD
            elif pix[i, j] == (0x6f, 0x6f, 0xa6, 255):
                mp[i, j] = HOUSE
            elif pix[i, j] == (0, 0x55, 0, 255):
                mp[i, j] = MOUNTAIN
            elif pix[i, j] == (0, 0xaa, 0, 255):
                mp[i, j] = FOREST
            else:
                r = random.randint(0, 100)
                if r <= 20:
                    mp[i, j] = PLAIN
                    pix[i, j] = (0x4d, 0xe7, 0, 255)
                elif r <= 50:
                    mp[i, j] = FOREST
                    pix[i, j] = (0, 0xaa, 0, 255)
                else:
                    mp[i, j] = MOUNTAIN
                    pix[i, j] = (0, 0x55, 0, 255)

    img.save('countryside2.png')
    store_txt('countryside3.in', mp)


def procYellowstone():
    img = Image.open('yellowstone1.png')
    pix = img.load()
    print(img.size)

    n = img.size[0]

    mp = np.zeros((n, n), dtype='int32')
    for i in range(n):
        for j in range(n):
            if pix[i, j] == (0x4d, 0xe7, 0, 255):
                mp[i, j] = PLAIN
            elif pix[i, j] == (0, 170, 255, 255):
                mp[i, j] = RIVER
            elif pix[i, j] == (0x98, 0x98, 0x98, 255):
                mp[i, j] = ROAD
            elif pix[i, j] == (0x6c, 0x65, 0x6d, 255):
                mp[i, j] = HOUSE
                pix[i, j] = (0x6f, 0x6f, 0xa6, 255)
            elif pix[i, j] == (0, 0x55, 0, 255):
                mp[i, j] = MOUNTAIN
            elif pix[i, j] == (0, 0xaa, 0, 255):
                mp[i, j] = FOREST
            else:
                r = random.randint(0, 100)
                mp[i, j] = FOREST
                pix[i, j] = (0, 0xaa, 0, 255)

    img.save('yellowstone2.png')
    store_txt('yellowstone3.in', mp)


def procCounty():
    img = Image.open('county1.png')
    pix = img.load()
    print(img.size)

    n = img.size[0]

    mp = np.zeros((n, n), dtype='int32')
    for i in range(n):
        for j in range(n):
            if pix[i, j] == (0, 170, 255, 255):
                mp[i, j] = RIVER
            elif pix[i, j] == (0x98, 0x98, 0x98, 255):
                mp[i, j] = ROAD
            elif pix[i, j] == (0x6f, 0x6f, 0xa6, 255):
                mp[i, j] = HOUSE
            else:
                mp[i, j] = 0
                pix[i, j] = (0, 0xaa, 0, 255)

    img.save('county2.png')
    store_txt('county3.in', mp)


def readBeijing():
    img = Image.open('./pic/beijing_ours.png')
    pix = img.load()

    n = img.size[0]
    mp = np.zeros((n, n), dtype='int32')
    for i in range(n):
        for j in range(n):
            if pix[i, j] == ROAD_T:
                mp[i, j] = ROAD
                pass
            elif pix[i, j] == RIVER_T:
                mp[i, j] = RIVER
                pass
            elif pix[i, j] == PLAIN_T:
                mp[i, j] = PLAIN
                pass
            elif pix[i, j] == FOREST_T:
                mp[i, j] = FOREST
                pass
            elif pix[i, j] == BUILDING_T:
                mp[i, j] = BUILDING
                pass
            elif pix[i, j] == HOUSE_T:
                mp[i, j] = HOUSE
                pass
            elif pix[i, j] == MOUNTAIN_T:
                mp[i, j] = MOUNTAIN
                pass
            else:
                print(pix[i, j])
                assert False
    store_txt('temp.in', mp)


readBeijing()
