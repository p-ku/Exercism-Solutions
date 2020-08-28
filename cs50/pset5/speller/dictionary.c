// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table, letters in the alphabet
const unsigned int N = 26;

// Hash table
node *table[N];
unsigned int count;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // find index
    unsigned int index = hash(word);
    if (index == N || table[index] == NULL)
    {
        return false;
    }
    node *cursor = table[index];
    do
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    while (cursor != NULL);
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    if (!isalpha(word[0]))
    {
        return N;
    }
    if ((int)word[0] > 90)
    {
        return (int)word[0] - 97;
    }
    else
    {
        return (int)word[0] - 65;
    }
    return N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    for (int i = 0; i < N; i++)
    {

    }
    count = 0;
    FILE *loads = fopen(dictionary, "r");
    // Check if dictionary loads
    if (loads == NULL)
    {
        return false;
    }
    int scans;
    char wor[LENGTH + 1];
    do
    {
        // allocate memory for node
        node *new_node = malloc(sizeof(node));
        // scan next word
        memset(wor, 0, sizeof(wor));
        scans = fscanf(loads, "%s", wor);
        strcpy(new_node->word, wor);
        if (hash(wor) >= 0 && hash(wor) <= 25)
        {
            new_node->next = table[hash(wor)];
            count += 1;
            table[hash(wor)] = new_node;
        }
        else
        {
            free(new_node);
        }
    }
    while (scans != EOF);
    fclose(loads);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cur = table[i];
        node *tmp = cur;

        while ((tmp = cur) != NULL)
        {
            cur = cur->next; // borrowed https://stackoverflow.com/questions/7025328/linkedlist-how-to-free-the-memory-allocated-using-malloc
            free(tmp);
        }
    }


    return true;
}
