from cs50 import get_int
# infinite loop until an appropriate input is given
while True:
    # get height input from user
    height = get_int("Height: ")
    # check if input is appropriate
    if height > 0 and height < 9:
        # continue only after appropriate input is given
        break
    # repeatedly loop until everything is drawn
for i in range(height, 0, -1):
    for j in range(0, i - 1):
        print(" ", end = "")
    for j in range(height - i + 1):
        print("#", end = "")
    print("  ", end = "")
    for j in range(height - i + 1):
        print("#", end = "")
    print()