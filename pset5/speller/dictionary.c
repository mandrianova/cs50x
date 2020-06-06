// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 12412445;

// Hash table
node *table[N];

unsigned int size_dictionary = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int lenght = strlen(word); //for tolower
    char *tmpword = NULL;
    tmpword = malloc(lenght + 1); //get memory for make a word tolower
    if (tmpword == NULL)
    {
        return false;
    }
    for (int i = 0; i < lenght; i++)
    {
        tmpword[i] = tolower(word[i]);
    }
    tmpword[lenght] = '\0';
    unsigned int h = hash(tmpword); //calculate hash
    if (h <= N && table[h] != NULL)
    {
        node *tmp_check = NULL;
        tmp_check = table[h]; //create tmp node for checking
        while (tmp_check != NULL)
        {
            if (!strcmp(tmp_check->word, tmpword))
            {
                free(tmpword);
                tmpword = NULL;
                tmp_check = NULL;
                return true;
            }
            else
            {
                tmp_check = tmp_check->next;
            }
        }
    }
    free(tmpword);
    tmpword = NULL;
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    if (word == NULL)
    {
        return 0;
    }
    int lenght = strlen(word);
    unsigned int h = 0;
    int k = 10;
    if (lenght < k)
    {
        k = lenght;
    }
    for (int i = 0; i < k; i++)
    {
        if ((int) word[i] > 95)
        {
            h += ((int) word[i] - 95) * 30 * i;
        }
    }
    h += lenght;
    return (int) h;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }
    char byte[1];
    int pointer = 0;
    char *word; //create variable for words
    int count_letter = 0;
    node *tmp = NULL; //create tmp variable for a next word in the table
    while (true)
    {
        fseek(dict, pointer, 0);
        fread(byte, 1, 1, dict);
        if (pointer == 0)
        {
            word = malloc(LENGTH + 1); //create place for first word
            if (word == NULL)
            {
                return false;
            }
        }
        pointer++;
        if (byte[0] == '\n' && !feof(dict)) //if a new word
        {
            word[count_letter] = '\0';
            unsigned int h = hash(word); // calculate hash for current word
            tmp = malloc(sizeof(node)); // create place for new word (for table)
            if (tmp == NULL)
            {
                return false;
            }
            strcpy(tmp->word, word); //copy word in tmp
            if (table[h] != NULL) //check table
            {
                tmp->next = table[h]; //link tmp and current word in table
            }
            else
            {
                tmp->next = NULL;
            }
            table[h] = tmp; //link table with word
            tmp = NULL; // dislink tmp
            size_dictionary++;
            count_letter = 0;
            free(word);
            word = malloc(LENGTH + 1);
            if (word == NULL)
            {
                return false;
            }
        }
        else if (!feof(dict))
        {
            word[count_letter] = byte[0];
            count_letter++;
        }
        if (feof(dict))
        {
            free(word);
            fclose(dict);
            break;
        }
    }
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return size_dictionary;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *tmp_unload = table[i]->next;
            free(table[i]);
            table[i] = tmp_unload;
        }
    }
    return true;
}
