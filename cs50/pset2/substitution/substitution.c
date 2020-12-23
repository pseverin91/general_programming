// Packages
#include <stdio.h>
#include <ctype.h>
#include <cs50.h>
#include <string.h>

// Main function
int main(int argc, string argv[])
{
    
    // 1. Test if correct number of arguments
    if (argc != 2)
    {
        printf("Error: Wrong argument\n");
        return 1;
    }
    
    // 2. Test if right argument length, character type, and duplicates
    if (strlen(argv[1]) != 26)
    {
        printf("Error: Wrong code length\n");
        return 1;
    }
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (argv[1][i] < 'A' || argv[1][i] > 'z' || (argv[1][i] > 'Z' && argv[1][i] < 'a'))
        {
            printf("Error: Wrong character in key\n");
            return 1;
        }
        for (int j = 0; j < i; j++)
        {
            if (argv[1][j] == argv[1][i])
            {
                printf("Error: Duplicates in key\n");
                return 1;
            }
        }
    }
    
    // Start encryption
    if (argc == 2)
    {
        // 3. Transform key to uppercase
        string key = argv[1];
        for (int i = 0; i < strlen(key); i++)
        {
            if (key[i] >= 'a' && key[i] <= 'z')
            {
                key[i] = toupper(key[i]);
            }
        }
        
        // 4. Ask for message for encryption
        string plain = get_string("Plaintext: ");
    
        // 5. Output translation
        printf("plaintext:  %s\n", plain);
        printf("ciphertext: ");
    
        char letter = 1;
    
        for (int i = 0; i < strlen(plain); i++)
        {
            if (plain[i] >= 'A' && plain[i] <= 'Z')
            {
                letter = key[(int) plain[i] - 65];
            }
            else if (plain[i] >= 'a' && plain[i] <= 'z')
            {
                letter = key[(int) plain[i] - 97];
                letter = letter + 32;
            }
            else
            {
                letter = plain[i];
            }
            printf("%c", letter);
        }
        printf("\n");
    }
    
}