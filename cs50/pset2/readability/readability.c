// Packages
#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>

// Main function
int main(void)
{
    // 1. Input text
    string text = get_string("Text: ");
    
    // 2. Compute the number of letters, words, and sentences
    float letters = 0;
    float words = 1;
    float sentences = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
        {
            letters++;
        }
        if (text[i] == ' ')
        {
            words++;
        }
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }
    //printf("%f letter(s)\n", letters);
    //printf("%f words(s)\n", words);
    //printf("%f sentences(s)\n", sentences);

    // 3. Compute Coleman-Liau index
    float L = letters / words * 100;
    float S = sentences / words * 100;
    float index = round(0.0588 * L - 0.296 * S - 15.8);
    //printf("L: %f\n", L);
    //printf("S: %f\n", S);
    //printf("Index: %f\n", index);
    
    // 4. Print out grade result
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    if (index >= 1 && index < 16)
    {
        printf("Grade %i\n", (int) index);
    }
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
}