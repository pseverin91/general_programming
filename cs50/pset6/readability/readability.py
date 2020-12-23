# Packages
from cs50 import get_string
import re

# Define main function
def main():

    # 1. Ask for text
    text = get_string('Text: ')

    # 2. Count letters, words, and sentences
    letters = 0
    words = 1
    sentences = 0

    for i in text:

        if re.search('[a-zA-Z]', i):
            letters += 1

        elif re.search(' ', i):
            words += 1

        elif re.search('[.!?]', i):
            sentences += 1

    # 3. Compute index
    L = letters / words * 100
    S = sentences / words * 100
    index = 0.0588 * L - 0.296 * S - 15.8

    # 4. Output corresponding text
    if index < 1:
        print("Before Grade 1")

    if index >= 1 and index < 16:
        print("Grade", round(index))

    if index >= 16:
        print("Grade 16+")

main()