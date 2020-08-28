import csv
from sys import argv, exit
# check if appropriate arguments provided
if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(1)

j = 0
# open both files for duration of program
with open(argv[2], newline='') as dnas:
    with open(argv[1], newline='') as csvfile:
        # begin reading
        reader = csv.reader(csvfile)
        # separate header
        headr = next(reader)
        # make dna into a string
        dnatxt = dnas.read()
        # array of counters to store number of repetitions for each column
        count = [0] * len(headr[1:])
        # loop through each patterm
        for i in headr[1:]:
            # find first pattern
            pattern = dnatxt.find(i)
            # search for increasingly larger patterns until no more are found
            while pattern != -1:
                count[j] += 1
                pattern = dnatxt.find(i * (count[j] + 1))
            j += 1
            # convert ints to strs within list
        txtarr = (list(map(str,count)))
        # loop through each row looking for match
        for row in reader:
            if txtarr == row[1:]:
                print(row[0])
                exit(0)
        print('No match')