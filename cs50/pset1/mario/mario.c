#include <stdio.h>
#include <cs50.h>

// Main function
int main(void)
{
    // Enter integer value between 1 and 8
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    // Run through lines
    for (int i = 0; i < n; i++)
    {
        // Run through columns
        for (int j = 0; j < n * 2 + 2; j++)
        {
            // Starting and ending spaces
            if (j < n - i - 1)
            {
                printf(" ");
            }
            // Hashtags
            else if (j < n || (j >= n + 2 & j <= n + 2 + i))
            {
                printf("#");
            }
            // Spaces inbetween
            else if (j >= n & j < n + 2)
            {
                printf(" ");
            }
        }
        // New line
        printf("\n");
    }
}
