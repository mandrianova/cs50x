height = 0
while height < 1 or height > 8:
    height = input("Height: ")  # ask height and check value
    try:
        height = int(height)
    except:
        height = 0

for i in range(1, height + 1):
    print(" " * (height - i), "#" * i, "  ", "#" * i, sep="")  # print pyramid