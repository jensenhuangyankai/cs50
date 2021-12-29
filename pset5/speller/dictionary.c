// Implements a dictionary's functionality
#define _GNU_SOURCE //required for getline
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;


/////////////////////////
// method
// https://www.hackerearth.com/practice/data-structures/hash-tables/basics-of-hash-tables/tutorial/
//////////////////////////

// Number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //logic here being: go to table[hashed_value]
    //if it is empty immediately return false
    //else traverse the linked list
    //if it is found, return true
    //else false at the end of the list
    int hashed_value = hash(word);
    if (table[hashed_value] == NULL)
    {
        //printf("returning false\n");
        return false;
    }
    else
    {
        //printf("word in table is %s\n", table[hashed_value]->word);
        //printf("%s\n", table[hashed_value]->next->word);
        if (strcasecmp(table[hashed_value]->word, word) == 0)
        {
            //printf("found1!\n");
            //printf("this is '%s' ,original word\n", word);
            //printf("this is '%s' sus word\n", table[hashed_value]->word);
            return true;
        }
        for (node *n = table[hashed_value] ; n->next != NULL; n = n->next)
        {
            //printf("%p", &n->word);
            //printf("word in following nodes are %s\n", n->next->next->word);
            if (strcasecmp(n->word, word) == 0)
            {
                //printf("%s, found in dict!\n", n->word);
                return true;
            }
            if (n->next->next == NULL) //checks the last term of the linked list too :)
            {
                if (strcasecmp(n->next->word, word) == 0)
                {
                    return true;
                }
            }
        }
        //printf("%s,not found in dict :( \n",word);
        return false;
    }
    printf("died\n");
    return false;
}



// Hashes word to a number
unsigned int hash(const char *word)
{
    int length = strlen(word);
    int total = 0;
    //printf("got here\n");
    for (int i = 0; i<strlen(word);i++)
    {
        total += tolower(word[i]) * i;
    }
    int result = total % 2069;
    //printf("%i\n",result);
    return result;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //char filename[80];
    //printf("hi\n");
    //sprintf(filename, "%s", dictionary);
    //printf("adfasd\n%sfewf\nwer",dictionary);
    //printf("tilt\n");


    FILE *f = fopen(dictionary,"r");

    //printf("hi\n");
    if (f == NULL)
    {
        printf("file import broken. exiting with error code 1\n");
        return false;
    }
    //printf("wow. dictionary was loaded\n");

    char word[LENGTH+1];

    //printf("\n%d\n",hash("A"));

    while (fscanf(f, "%s", word) != EOF)
    {
        //printf("reading 1 line. line is %s \n", word);
        int hashed_value = hash(word);
        //printf("hashed_value is %d\n", hashed_value);
        //printf("%p\n", table[hashed_value]);
        if (table[hashed_value] == NULL) //empty area. not even a node is here
        {
            //printf("no node found! creating one now\n");
            node *temp_node = malloc(sizeof(node));
            temp_node->next = NULL;
            //printf("%s\n", word);
            strcpy(temp_node->word , word);
            table[hashed_value] = temp_node;
            //printf("copied '%s' into hash table\n",temp_node->word);
        }
        else
        {
            //checking if hashed_value is empty
            //else insert into front of list
            node *temp_node = malloc(sizeof(node));
            strcpy(temp_node->word , word);
            temp_node->next = table[hashed_value];
            table[hashed_value] = temp_node;
            //printf("copied '%s' into hash table\n",temp_node->word);
            //printf("next word %s",temp_node->next->word);
            //printf("next word %s",temp_node->next->next->word);
            //printf("%s\n", word);
        }

    }
    fclose(f);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    //iterate over hash table. follow the linked list until the next number == NULL
    //do that until the last index == null too
    //lets just try with N = 1000 for now
    //turns out 1000 is too small. maybe find a better case in the middle somehow?
    int total = 0;
    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            continue;
        }
        node *n = table[i];

        if (n->next != NULL)
        {
            //checking if node is empty
            //else go to next node in linked list
            for (node *t = table[i]; t->next != NULL; t = t->next)
            {
                total += 1;
            }
        }
        else
        {
            total += 1;
        }
    }
    return total;

}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        //printf("%d\n", i);

        //free(table[2069]);
        //printf("%p\n", table[i]);
        if (table[i] == NULL)
        {
            //printf("was here");
            free(table[i]);
        }
        else if (table[i]->next != NULL)
        {
            //printf("node not empty\n");
            //checking if node is empty
            //else go to next node in linked list
            for (; table[i]->next != NULL; table[i] = table[i]->next)
            {
                //printf("sike was here");
                //printf("freed '%s'\n", table[i]->word );
                free(table[i]);

                //printf("1\n");
            }
        }
        //printf("nope, actually here");
        free(table[i]);
        //printf("%s", table[2069]->word);
    }

    //printf("freed all lol");
    return true;
}
