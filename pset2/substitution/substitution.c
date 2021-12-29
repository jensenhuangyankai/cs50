#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc > 2 || argc == 1)
    {
        printf("%s", "Usage: ./substitution key\n");
        return 1;
    }

    if (strlen(argv[1]) == 26)
    {
        bool isalphabet = true;
        int count[999] = {0};
        for (int i = 0; i < 26; i++)
        {
            if (isalpha(argv[1][i]) == 0)
            {
                printf("%s", "Usage: ./substitution key\n");
                return 1;
            }
            count[tolower(argv[1][i])-97]++;
        }
        for (int i = 0; i < 26; i++)
        {
            if (count[i] > 1)
            {

                printf("%s", "Usage: ./substitution key\n");
                return 1;
            }
        }
        printf("%s", "plaintext: ");
        char *input = malloc(9999);
        char output[9999];

        fgets(input, 256, stdin);
        for (int i = 0; i < strlen(input); i++)
        {
            if (islower(input[i]))
            {
                output[i] = tolower(argv[1][input[i]-97]);
                //printf("%c%s", output[i],"lmao");
            }
            else if (isupper(input[i]))
            {
                output[i] = toupper(argv[1][input[i]-65]);
                //printf("%c", output[i]);
            }
            else output[i] = input[i];
        }

        printf("%s%s\n", "ciphertext: " , output);


    }

    else if (strlen(argv[1]) != 26)
    {
        printf("%s", "Key must contain 26 characters.\n");
        return 1;
    }

    else
    {
        printf("%s", "Usage: ./substitution key\n");
        return 1;
    }
}