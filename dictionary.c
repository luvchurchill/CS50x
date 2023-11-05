// Implements a dictionary's functionality
// I absolutely cannot believe I was able to do this!

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in hash table
// 26 * 46 * 46 plus extra just in case
const unsigned int N = 56200;

// Hash table
node *table[N];

// Global variable storing dictionary, size of dict, file and the hashtable
char *dict_ptr;
long size_in_bytes;
FILE *dict;
node *nmall;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int hashed_word = hash(word);
    node *ptr = table[hashed_word];
    while (ptr != NULL)
    {
        if (strcasecmp(word, table[hashed_word]->word) == 0)
        {
            return true;
        }
        else
        {
            ptr = ptr->next;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Hash function adds value for each letter in word then multiplies by length
    int letter_values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};
    unsigned int hash_value = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        hash_value += letter_values[tolower(word[i]) - 97];
    }
    hash_value = hash_value * strlen(word);
    return hash_value;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open file
    dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }
    // Find size of dictionary
    int end_of_dictionary = fseek(dict, 0, SEEK_END);
    if (end_of_dictionary != 0)
    {
        printf("fseek didn't work\n");
        return 1;
    }
    size_in_bytes = ftell(dict);
    if (size_in_bytes < 0)
    {
        printf("ftell didn't work\n");
        return 1;
    }
    fseek(dict, 0, SEEK_SET);
    // Allocate memory for Dictionary
    dict_ptr = malloc(sizeof(char) * size_in_bytes);
    if (dict_ptr == NULL)
    {
        printf("Bad Malloc/no memory\n");
        return 1;
    }
    // Read contents
    long bytes_read = fread(dict_ptr, sizeof(char), size_in_bytes, dict);
    if (bytes_read != size_in_bytes)
    {
        printf("bad fread\n");
        return 1;
    }

    // Arrange into hashtable

    unsigned int size_var = size();

    // For each word in dictionary
    int i = 0;
    // Keeps track of where I am in the dictionary
    int position = 0;
    while (i < size_var)
    {
        char wordhere[LENGTH + 1];
        // For each char in word
        for (int j = 0; j < LENGTH + 1; j++)
        {
            // If word is not done yet
            if (dict_ptr[position + j] != '\n')
            {
                // Add to wordhere
                wordhere[j] = dict_ptr[position + j];
            }
            else
            {
                nmall = malloc(sizeof(node));
                if (nmall == NULL)
                {
                    return 1;
                }
                wordhere[j] = '\0';
                // Add word to node
                strcpy(nmall->word, wordhere);
                nmall->next = NULL;
                // New node points to old node
                nmall->next = table[hash(wordhere)];
                // Hashtable[N] points to new node
                table[hash(wordhere)] = nmall;

                position = position + j + 1;
                break;
            }
        }
        i++;
    }

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int size = 0;
    int counter = 1;
    while (true)
    {
        if (counter == size_in_bytes)
        {
            break;
        }
        else if (dict_ptr[counter] == '\n')
        {
            size++;
        }
        counter++;
    }
    return size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Close file and free dictionary
    fclose(dict);
    free(dict_ptr);
    // Free hashtable
    for (int i = 0; i < N; i++)
    {
        node *nptr = table[i];
        while (nptr != NULL)
        {
            node *next = nptr->next;
            free(nptr);
            nptr = next;
        }
    }

    return true;
}
