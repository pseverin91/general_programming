// Implements a dictionary's functionality

#include <stdbool.h>

#include "dictionary.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Word count
long count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // 1. Hash word to obtain hash value
    int hash_value = hash(word);
    if (word[0] >= 'A' & word[0] <= 'Z')
    {
        hash_value = hash_value + 32;
    }

    // 2. Access linked list at that index in the hash table
    node *current = table[hash_value];
    
    // 3. Traverse linked list, looking for the word
    while (current != NULL)
    {
        
        //printf("Word: %s\n", word);
        //printf("Dictionary Word: %s\n", current->word);
        if (strcasecmp(word, current->word) == 0)
        {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Return index from 0 to 25 for first letters in word
    return (int) word[0] - 97;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // 1. Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (!file)
    {
        return 1;
    }

    // 2. Read strings from file
    char word[LENGTH];
    node *temp = NULL;
    node *p = NULL;
    
    while (fscanf(file, "%s", word) != EOF)
    {
        //printf("%s\n", word);
        count++;
        
        // 3. Create a new node for each word
        // Create isolated node
        temp = malloc(sizeof(node));
        //printf("Count: %li\n", count);
        if (temp == NULL)      // Check for available memory
        {
            return 1;
        }
        strcpy(temp->word, word);
        temp->next = NULL;

        // 4. Hash word to obtain has value
        int hash_value = hash(word);

        // 5. Insert node into hash table at that location
        // #Start of linked list
        if (table[hash_value] == NULL)
        {
            table[hash_value] = temp;
        }
        
        // #Add isolated node
        else
        {
            p = table[hash_value];
            while (p->next != NULL)
            {
                p = p->next;
            }
            p->next = temp;
        }
    }
    
    // Test linked list
    node *current = table[2];
    while (current != NULL)
    {
        //printf("Word: %s\n", current->word);
        current = current->next;
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *current = table[i];
        node *tmp = table[i];
        while (current != NULL)
        {
            tmp = tmp->next;
            free(current);
            current = tmp;
        }
    }
    return true;
}
