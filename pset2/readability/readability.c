#include <stdio.h>
#include <math.h>
#include <string.h>

int main(void)
{
    printf("%s", "Text: ");
    char text[9999];
    scanf("%[^\n]%*c", text);

    int letters = 0, words = 0, sentences = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        printf("%d\n",i);
        if (text[i] == '\'') i++;
        if (text[i] == ',')
        {
            letters--;
        }

        if (text[i] == '.' || text[i] == '?' || text[i] == '!' )
        {
            sentences++;
            words++;
            i++;

        }

        else if (text[i] == ' ') words++;

        else letters++;



    }

    printf("%i\n%i\n%i\n", letters, sentences,words);
    float L = letters * (100.0/words);
    //printf("%f\n",L);
    float S = sentences * (100.0/words);
    //printf("%f\n",S);
    float index = 0.0588 * L - 0.296 * S - 15.8;
    //printf("%f\n",index);
    //printf("%f\n", index);
    //printf("%f\n%f", L,S);

    if (index < 1)
    {
        printf("%s", "Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("%s", "Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(index));
    }


}