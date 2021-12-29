#include <cs50.h>
#include <stdio.h>
#include <string.h>


int main(void)
{
    int height;
    {
        printf("%s", "Height: ");
        scanf("%i", &height);
    } while (height <= 0 || height >= 9);
    for (int i = 1; i < height + 1; i++)
    {
        char final_string[9999];
        for (int j = 0; j < height; j++)
        {
            int spaces = height - i;
            if (j < spaces)
            {
                final_string[j] = ' ';
            }
            if (j >= spaces && j < height)
            {
                final_string[j] = '#';
            }
        }
        final_string[height] = ' ';
        final_string[height + 1] = ' ';
        for (int j = height + 2; j < height + 2 + i; j++)
        {
            final_string[j] = '#';

        }
        printf("%s\n", final_string);
    }
}
