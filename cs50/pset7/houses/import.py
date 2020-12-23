# Packages
import sys
import csv
from cs50 import SQL

def main():

    # 1. Check command-line arguments
    if len(sys.argv) != 2:
        print('Execute with one input: python import.py [csv-file]')
        exit(1)

    # 2. Open csv-file given by command-line argument
    with open(sys.argv[1], 'r') as file:
        data = list(csv.reader(file))

    # 3. For each row, parse name
    data[0] = ['first', 'middle', 'last', 'house', 'birth']
    for i in range(1, len(data)):

        row_new = data[i][0].split(' ')
        if len(row_new) == 2:
            row_new.insert(1, None)

        row_new.append(data[i][1])
        row_new.append(data[i][2])

        data[i] = row_new

    # 4. Insert each student into the students table
    db = SQL("sqlite:///students.db")
    db.execute("DELETE FROM students")
    for row in data[1:]:
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", row)
    exit(0)

main()