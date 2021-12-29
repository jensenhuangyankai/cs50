#include<cs50.h>
#include<stdio.h>
#include<math.h>

int main(void)
{
    float owed_dollars;
    do
    {
    owed_dollars = get_float("Change owed: ");
    } while (owed_dollars < 0);

    int owed_cents = round(owed_dollars * 100);
    int counter = 0;

    while (owed_cents > 0)
    {
        if (owed_cents - 25 >= 0)
        {
            owed_cents -= 25;
            counter += 1;
        }

        else if (owed_cents - 10 >= 0)
        {
            owed_cents -= 10;
            counter += 1;
        }

        else if (owed_cents - 5 >= 0)
        {
            owed_cents -= 5;
            counter += 1;
        }

        else
        {
            owed_cents -= 1;
            counter += 1;
        }

    }

    printf("%i\n", counter);


}