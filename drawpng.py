from PIL import Image, ImageColor

with open('terrain.in') as f:
    n = [int(x) for x in next(f).split()][0]
    array = []
    for line in f:
        array.append([int(x) for x in line.split()])
        if len(array) == n:
            break

ROAD = 10
RIVER = 20
PLAIN = 30
FOREST = 40
BUILDING = 50
HOUSE = 60
MOUNTAIN = 70

img = Image.new('RGB', (n, n))

size = 1


def draw(ty, x, y):
    for i in range(size):
        for j in range(size):
            s = "#ffffff"
            if ty == ROAD:
                s = "#989898"
            if ty == RIVER:
                s = "#00aaff"
            if ty == PLAIN:
                s = "#4de700"
            if ty == FOREST:
                s = "#00aa00"
            if BUILDING <= ty and ty <= BUILDING + 2:
                s = "#55557f"
            if ty == HOUSE:
                s = "#6f6fa6"
            if ty == MOUNTAIN:
                s = "#005500"

            img.putpixel((x + i, y + j), ImageColor.getrgb(s))


for i in range(n):
    for j in range(n):
        draw(array[i][j], j * size, (n - 1 - i) * size)

img.save('simplePixel.png')
