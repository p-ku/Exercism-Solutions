# import cs50 library to use get_string to prompt user for string
from cs50 import get_string
# prompt user for a string, their name
name = get_string("What is your name?\n")
# print the name and say hello
print(f"hello, {name}")