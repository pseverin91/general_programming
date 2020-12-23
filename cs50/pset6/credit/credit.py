# Packages
from cs50 import get_int



def main():
    # 1. Ask for number and put to string
    number = get_int('Number: ')
    if int(number) < 0:
        number = get_int('Number: ')
    number = str(number)

    # 2. Check number algorithm
    product = 0
    sum = 0
    
    #Compute products
    for i in range(len(number)):
        if i % 2 == 1:
            digit = str(2 * int(number[len(number) - i - 1]))
            for j in range(len(digit)):
                product += int(digit[j])
    
    #Compute sum
        if i % 2 == 0:
            sum += int(number[len(number) - i - 1])
    
    #Test general validity
    test = str(product + sum)
    if int(test[len(test) - 1]) != 0:
        print('INVALID')
    
    # 3. Detect card type
    else:
        if len(number) == 15 and int(number[0:2]) in [34, 37]:
            print('AMEX')
            
        elif len(number) == 16 and int(number[0:2]) in [51, 52, 53, 54, 55]:
            print('MASTERCARD')
    
        elif (len(number) == 13 or len(number) == 16) and int(number[0]) == 4:
            print('VISA')
            
        else:
            print('INVALID')

main()
