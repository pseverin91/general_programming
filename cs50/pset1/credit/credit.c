#include <stdio.h>
#include <cs50.h>

long power(int n);
int digits(long number);

int main(void)
{
    
    // ### 1. Get number input
    long number;
    do
    {
        number = get_long("Number: ");
    }
    while (number < 0);
    
    // ### 2. Check number algorithm
    int sum = 0;
    int n = digits(number);
    
    // Go through all digits
    for (int i = 0; i < n; i++)
    {
        int digit = (number % power(i + 1)) / power(i);
        
        // Multiply every other digit by two and sum
        if (i % 2 == 1)
        {
            int number_new = digit * 2;
            for (int j = 0; j < digits(number_new); j++)
            {
                int digit_new = (number_new % power(j + 1)) / power(j);
                sum += digit_new;
            }
        }
        
        // Sum every other digit
        else
        {
            sum += digit;
        }
    }
    
    // Validate if last digit of sum is 0
    if (sum % 10 != 0)
    {
        printf("INVALID\n");
    }
    
    // ### 3. Check card type
    else
    {
        if ((n == 15) & (number / power(digits(number) - 2) == 34 || number / power(digits(number) - 2) == 37))
        {
            printf("AMEX\n");
        }
        else if ((n == 16) & (number / power(digits(number) - 2) > 50) & (number / power(digits(number) - 2) <= 55))
        {
            printf("MASTERCARD\n");
        }
        else if (((n == 13) || (n == 16)) & (number / power(digits(number) - 1) == 4))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }

}

// Function to get the power (10^x) of a number
long power(int n)
{
    long value = 1;
    for (int i = 0; i < n; i++)
    {
        value = value * 10;
    }
    return value;
}

// Function to get the number of digits of a number
int digits(long number)
{
    int digit = 0;
    while (number > 0)
    {
        number = number / 10;
        digit++;
    }
    return digit;
}