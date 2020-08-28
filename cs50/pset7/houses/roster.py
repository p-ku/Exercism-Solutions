import cs50
from sys import argv, exit
# check if appropriate arguments provided
if len(argv) != 2:
    print("Usage: roster.py house")
    exit(1)

db = cs50.SQL("sqlite:///students.db")
outs = db.execute("SELECT first, middle, last, birth FROM students WHERE house = '"+argv[1]+"' ORDER BY last, first;")
for row in outs:
    if row['middle'] == None:
        print(row['first'] + ' ' + row['last'] + ', born ' + str(row['birth']))
    else:
        print(row['first'] + ' ' + row['middle'] + ' ' + row['last'] + ', born ' + str(row['birth']))