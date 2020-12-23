#include <stdio.h>
#include <cs50.h>

// Main function
int main(void)
{
    // Input name
    string name = get_string("What is your name?");
    
    // Say hello
    printf("Hello, %s\n", name);
}