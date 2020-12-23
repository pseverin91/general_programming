# Packages
import sys
from cs50 import SQL

def main():
    
    # 1. Check command-line arguments
    if len(sys.argv) != 2:
        print('Execute with one input: python roster.py [house]')
        exit(1)
    
    # 2. Query database for all students in house
    db = SQL("sqlite:///students.db")
    data = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last, first", sys.argv[1])
    
    # 3. Print out each student's full name and birth year (sorted by lastname and firstname)
    for row in data:
        if row['middle'] != None:
            print(row['first'], row['middle'], row['last'] + ', born', row['birth'])
        else:
            print(row['first'], row['last'] + ', born', row['birth'])

main()