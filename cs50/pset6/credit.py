from cs50 import get_int
# initialize variables, to include card number, digit count, and sums for algorithm
ccnum = get_int("Number: ")
check = ccnum
count = 0
summed = 0
p = []
# loop to determine how many digits are in the number
while check != 0:
    pnext = check % 10
    p.append(pnext)
    check = (check - p[count]) / 10
    count += 1
# add an empty element in case loop extends beyond size of array since number could be even or odd
p.append(0)
# do algo math, which goes between alternate digits of the number
for i in range(0, count + 1, 2):
    summed = summed + p[i]
for i in range(1, count + 1, 2):
    if 2 * p[i] > 9:
        summed += 2 * p[i] - 9
    else:
        summed += 2 * p[i]
    #do checks to determine which card we have and print the results
if summed % 10 == 0:
    if p[count - 1] == 4 and (count == 16 or count == 13):
        print("VISA")
    elif count == 15 and p[count - 1] == 3 and (p[count - 2] == 4 or p[count - 2] == 7):
        print("AMEX")
    elif count == 16 and p[count - 1] == 5 and (p[count - 2] == 1 or p[count - 2] == 2 or p[count - 2] == 3 or p[count - 2] == 4 or p[count - 2] == 5):
        print("MASTERCARD")
    else:
        print("INVALID")
else:
    print("INVALID")