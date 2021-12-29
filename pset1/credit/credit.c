#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


int main(void)
{   
    bool valid = false;
    long card_int;
    printf("%s", "Number: \n");
    scanf("%ld", &card_int);
    char card_string[9999];
    sprintf(card_string, "%ld", card_int);
    int length = strlen(card_string);
    
    long card_number[99999];
    long long int x = card_int;
    for (int i = length - 1; i > -1 ; i--)
    {
        card_number[i] = x % 10; //breaking down a number into an array of individual digits
        x = floor(x / 10);  
    }

    //validity testing
    if (length != 16 && length != 15 && length != 13)
    {
        printf("%s", "INVALID\n");
    }
    else
    {   
        int products_digits_sum = 0;
        for (int i = length - 2 ; i >= 0 ; i-=2)
        {
            card_number[i] *= 2;
            if (card_number[i] > 9)
            {
                //number has to be below 20
                products_digits_sum += card_number[i] % 10;
                products_digits_sum += floor(card_number[i] / 10);
            }
            else
            {
                products_digits_sum += card_number[i];
                
            }
            card_number[i] /= 2;
        }

        int full_sum = products_digits_sum;
        
        //step 2
        for (int i = length - 1 ; i >= 0 ; i-=2)
        {
            full_sum += card_number[i];
        }

        //step 3
        if (full_sum % 10 == 0)
        {
            valid = true;
        }

        if (valid)
        {
            
            //AMEX
            if (card_number[0] == 3 && (card_number[1] == 4 || card_number[1] == 7)) printf("%s", "AMEX\n");

            //VISA
            else if (card_number[0] == 4) printf("%s", "VISA\n");
            
            //MASTERCARD
            else if (card_number[0] == 5 && (card_number[1] == 1 || card_number[1] == 2 || card_number[1] == 3 || card_number[1] == 4 || card_number[1] == 5)) printf("%s", "MASTERCARD\n");

            else printf("%s", "INVALID\n");

        }
        else
        {
            printf("%s", "INVALID\n");
        }

    }

}