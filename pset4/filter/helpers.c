#include "helpers.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<math.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE *pixel = &image[i][j];
            float float_avg = round((pixel->rgbtBlue + pixel->rgbtRed + pixel->rgbtGreen)/3.0);
            //printf("%f",float_avg);
            int int_avg = float_avg;
            //printf("%i",round_avg);
            pixel->rgbtBlue = int_avg;
            pixel->rgbtRed = int_avg;
            pixel->rgbtGreen = int_avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++) //replace 5 with height
    {
        for (int j = 0; j < width/2; j++) //replace 5 with width/2
        {
            RGBTRIPLE *pixel = &image[i][j];
            RGBTRIPLE *opp_pixel = &image[i][width-j-1];
            RGBTRIPLE *temp = malloc(3);
            //printf("\nInitial pixel is %p\n", pixel);
            //printf("Initial opp_pixel is %p\n", opp_pixel);
            memcpy(temp, opp_pixel, 3);

            memcpy(opp_pixel,pixel,3);

            memcpy(pixel,temp,3);
            free(temp);
            //temp = opp_pixel;
            //opp_pixel = pixel;
            //pixel = temp;
            //printf("Final pixel is %p\n", pixel);
            //printf("Final opp_pixel is %p\n", opp_pixel);
        }
    }

    return;
}

// Blur image // I AM A GOD
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    /* LOGIC
    iterate over the image and save everything into temp_image
    then once done, iterate over temp_image and save everything back into image
    */
    RGBTRIPLE temp_image[height][width]; //temporary image to save values into
    for (int i = 0; i < height; i++) // iterating over the image pixel by pixel
    {
        for (int j = 0; j < width; j++)
        {
            float totalRed = 0, totalBlue = 0, totalGreen = 0;
            float avgRed = 0, avgBlue = 0, avgGreen = 0;
            float counter = 0;
            //iterate over the 3x3 grid around that one pixel
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    int xValue = i+x;
                    int yValue = j+y;
                    if (xValue >= 0 && xValue < height && yValue >= 0 && yValue < width)
                    {
                        RGBTRIPLE *pixel = &image[xValue][yValue];
                        float pixelRed = pixel->rgbtRed;
                        float pixelBlue = pixel->rgbtBlue;
                        float pixelGreen = pixel->rgbtGreen;
                        totalRed += pixelRed;
                        totalBlue += pixelBlue;
                        totalGreen += pixelGreen;
                        counter++;
                    }
                }
            }

            avgRed = round(totalRed / counter);
            avgBlue = round(totalBlue / counter);
            avgGreen = round(totalGreen / counter);

            temp_image[i][j].rgbtRed = avgRed;
            temp_image[i][j].rgbtBlue = avgBlue;
            temp_image[i][j].rgbtGreen = avgGreen;


        }
    }

    for (int i = 0; i < height; i++) // copying over the image pixel by pixel
    {
        for (int j = 0; j < width; j++)
        {
            memcpy(&image[i][j] , &temp_image[i][j] , 3);
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp_image[height][width];
    for (int i = 0; i < height; i++) // iterating over the image pixel by pixel
    {
        for (int j = 0; j < width; j++)
        {
            //RGBTRIPLE *pixel = &image[i][j];
            int redGx = 0, redGy = 0;
            int greenGx = 0, greenGy = 0;
            int blueGx = 0, blueGy = 0;
            float redResult = 0, blueResult = 0, greenResult = 0;
            //iterate over the 3x3 grid around that one pixel

            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    int xValue = i+x;
                    int yValue = j+y;

                    /*
                    //check if value is outside the border of image. if so, treat as black.
                    if (xValue < 0 || xValue > height || yValue < 0 || yValue > width)
                    {
                        redGx += 0;
                        blueGx += 0;
                        greenGx += 0;

                        redGy += 0;
                        blueGy += 0;
                        greenGy += 0;
                    }
                    */

                    if (xValue >= 0 && xValue < height && yValue >= 0 && yValue < width)
                    {
                        RGBTRIPLE *pixel = &image[xValue][yValue];

                        float pixelRed = pixel->rgbtRed;
                        float pixelBlue = pixel->rgbtBlue;
                        float pixelGreen = pixel->rgbtGreen;

                        /////////////////////////
                        // Gx calculation
                        int factor1 = 0, factor2 = 0;

                        factor1 = x;
                        if (y == 0)
                        {
                            factor2 = 2;
                        }
                        else
                        {
                            factor2 = 1;
                        }

                        blueGx += pixelBlue * factor1 * factor2;
                        redGx += pixelRed * factor1 * factor2;
                        greenGx += pixelGreen * factor1 * factor2;

                        //Gx END
                        ////////////////////////


                        ////////////////////////
                        //Gy calculation
                        factor1 = 0, factor2 = 0;

                        factor1 = y;

                        if (x == 0)
                        {
                            factor2 = 2;
                        }
                        else
                        {
                            factor2 = 1;
                        }

                        blueGy += pixelBlue * factor1 * factor2;
                        redGy += pixelRed * factor1 * factor2;
                        greenGy += pixelGreen * factor1 * factor2;

                        //Gy END
                        ////////////////////////////

                        /*
                        //////
                        //Gx//
                        //Gx//
                        //////
                        if (y == 0) //detect if its the middle row. if middle row, y * 2
                        {

                            redGx += y * 2 * pixelRed;
                            blueGx += y * 2 * pixelBlue;
                            greenGx += y * 2 * pixelGreen;

                        }
                        else
                        {
                            redGx += y * pixelRed;
                            blueGx += y * pixelBlue;
                            greenGx += y * pixelGreen;
                        }

                        //gy calculation
                        if (x == 0)
                        {
                                redGy += x * 2 * pixelRed;
                                blueGy += x * 2 * pixelBlue;
                                greenGy += x * 2 * pixelGreen;
                        }
                        else
                        {
                            redGy += x * pixelRed;
                            blueGy += x * pixelBlue;
                            greenGy += x * pixelGreen;

                        }
                        */
                    }


                }
            }
            //GxGy calculation
            //square root of Gx^2 + Gy^2
            redResult = round(sqrt(pow(redGx,2) + pow(redGy,2)));
            blueResult = round(sqrt(pow(blueGx,2) + pow(blueGy,2)));
            greenResult = round(sqrt(pow(greenGx,2) + pow(greenGy,2)));

            if (redResult > 255)
            {
                redResult = 255;
            }
            if (greenResult > 255)
            {
                greenResult = 255;
            }
            if (blueResult > 255)
            {
                blueResult = 255;
            }



            temp_image[i][j].rgbtRed = redResult;
            temp_image[i][j].rgbtBlue = blueResult;
            temp_image[i][j].rgbtGreen = greenResult;
        }

    }

    for (int i = 0; i < height; i++) // copying over the image pixel by pixel
    {
        for (int j = 0; j < width; j++)
        {
            memcpy(&image[i][j] , &temp_image[i][j] , 3);
        }
    }

    return;
}
