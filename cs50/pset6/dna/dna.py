# Packages
import sys
import csv
import re


def main():
    
    # Check input
    if len(sys.argv) != 3:
        print('Execute with two inputs: python dna.py [database] [sequence]')
        exit(1)
    
    # Open csv-file and txt-file
    file = open(sys.argv[1], 'r')
    database = list(csv.reader(file))
    
    file = open(sys.argv[2], 'r')
    dna = file.read()
    
    # Test dna for pattern
    pattern_freq = []
    for pattern in database[0][1:]:

        count = 0
        dna_tmp = dna
        match = re.search(f'({pattern})+', dna_tmp)
        
        while match:
            
            count = max(count, len(match.group()) / len(pattern))
            dna_tmp = dna_tmp[match.end():]
            match = re.search(f'({pattern})+', dna_tmp)
        
        pattern_freq.append(count)
    
    # Compare with database
    for i in range(1, len(database)):
        count = 0
        for j in range(1, len(database[0])):

            if int(pattern_freq[j - 1]) == int(database[i][j]):
                count += 1
        
        # Print out person if successful, else no match
        if count == len(database[0]) - 1:
            print(database[i][0])
            exit(0)

    print('No match')
    exit(0)


main()