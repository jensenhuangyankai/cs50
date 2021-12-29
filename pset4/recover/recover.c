#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("go away use 1 arg only");
        return 1;
    }

    //open file. if broken, return 1
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    int counter = 0;

    //LOGIC
    //put 512 Bytes into a buffer at one time
    //scan the first 4 Bytes for presence of JPEG
    //if no jpeg, write buffer to output file
    //else start new file

    char outName[90] = "000.jpg";
    sprintf(outName , "%03d.jpg" , counter);
    printf("%s",outName);
    FILE *output = fopen(outName, "w");

    bool started = false;

    uint8_t buffer[512];
    while (fread(&buffer, 1, 512 ,input) == 512)
    {

        // the first three bytes of JPEGs are 0xff 0xd8 0xff
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff)) //maybe jpeg
        {
            //printf("%x %x %x %x \n",buffer[0],buffer[1],buffer[2],(buffer[3]>>4));

            if ((buffer[3] >> 4) == 0xe) //confirm 4th byte's first 4 bits is 1110. confirm jpeg
            {

                //printf("starting image %s" , outName);
                started = true;
                //printf("%x %x %x \n",buffer[0],buffer[1],buffer[2]);

                if (counter > 0) //handling everything after 000
                {
                    fclose(output);

                    sprintf(outName , "%03d.jpg" , counter); //%03d does minimum 3 digits so it pads zeros!
                    counter++;
                    output = fopen(outName, "w");
                    fwrite(&buffer, 1,512,output);
                }
                else //handling 000.jpg
                {
                    fwrite(&buffer, 1,512,output);
                    counter++;
                }

            }
        }
        else
        {
            if (output == NULL || started == false)
            {
                continue;
            }
            else
            {
                fwrite(&buffer, 1,512,output);
            }
        }
    }

    fclose(input);
    fclose(output);
}