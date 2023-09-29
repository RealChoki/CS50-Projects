from cs50 import get_int

# TODO
while True:
    height = get_int("Height: ")
    if height >= 1 and height <= 8:
        break


for i in range(height):
    for j in range(height - i - 1):
        print(" ", end="")
    for j in range(i + 1):
        print("#", end="")

    print("  ", end="")

    for j in range(i + 1):
        print("#", end="")

    print("")
