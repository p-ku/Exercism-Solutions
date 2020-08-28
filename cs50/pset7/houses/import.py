import cs50
import csv
from sys import argv, exit

# check if appropriate arguments provided
if len(argv) != 2:
    print("Usage: import.py characters.csv")
    exit(1)

open("students.db", "w").close()

db = cs50.SQL("sqlite:///students.db")

db.execute("CREATE TABLE students (id INT, first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC)")

with open(argv[1], "r") as csvfile:
    # begin reading
    reader = csv.reader(csvfile)
    # separate header
    headr = next(reader)
    idcount = 0
    full = [0] * 3
    for row in reader:
        full = row[0].split()
        if len(full) == 2:
            db.execute("INSERT INTO students ( id, first, middle, last, house, birth ) VALUES(?, ?, ?, ?, ?, ?)",
                idcount, full[0], None, full[1], row[1], row[2])
        else:
            db.execute("INSERT INTO students ( id, first, middle, last, house, birth ) VALUES(?, ?, ?, ?, ?, ?)",
                idcount, full[0], full[1], full[2], row[1], row[2])