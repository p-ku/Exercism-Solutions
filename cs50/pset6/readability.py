from cs50 import get_string
import string
# input string from user
text = get_string("Text: ")
# retrieve a count of letters, words, and sentences
lcount = sum(c in string.ascii_letters for c in text)
wcount = text.count(" ") + 1
scount = text.count("!") + text.count(".") + text.count("?")
# Do the math
L = lcount / (wcount / 100)
S = scount / (wcount / 100)
index =  0.0588 * L - 0.296 * S - 15.8 + 0.5
# Check against math to determine appropriate level
if index < 1:
    print("Before Grade 1")
elif index > 15:
    print("Grade 16+")
else:
    print(f"Grade {int(index)}")