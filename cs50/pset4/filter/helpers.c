#include "helpers.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // tricks to round (even tho I have math.h), I added half the predivided value, then divided. Magic.
            unsigned int average = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed + (3 / 2)) / 3;
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp;
    // Only needed to loop half the size of the image (quarter I guess), did full swap each time
    for (int i = 0, half = width / 2; i < height; i++)
    {
        for (int j = 0; j < half; j++)
        {
            tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE newimage[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // default values represent a middle pixel case
            int ilo = -1;
            int ihi = 1;
            int jlo = -1;
            int jhi = 1;
            int edge = 0;
            unsigned int divisor = 9;
            int avesumblue = 0;
            int avesumgreen = 0;
            int avesumred = 0;
            // these checks are determining if there are edges and corners, a corner has 2 edges.
            if (i == 0)
            {
                // the ilo, jlo etc. values are used to ignore appropriate pixels near edges
                ilo = 0;
                edge += 1;
            }
            if (i == height - 1)
            {
                ihi = 0;
                edge += 1;
            }
            if (j == 0)
            {
                jlo = 0;
                edge += 1;
            }
            if (j == width - 1)
            {
                jhi = 0;
                edge += 1;
            }
            if (edge == 2)
            {
                divisor = 4;
            }
            else if (edge == 1)
            {
                divisor = 6;
            }
            // smaller loop to loop through adjacent pixels
            for (int k = ilo; k <= ihi; k++)
            {
                for (int l = jlo; l <= jhi; l++)
                {
                    avesumblue += image[i + k][j + l].rgbtBlue;
                    avesumgreen += image[i + k][j + l].rgbtGreen;
                    avesumred += image[i + k][j + l].rgbtRed;
                }
            }
            // complete calculation for a single pixel in a completely new image since I don't want to dirty my copy of the original
            newimage[i][j].rgbtBlue = (avesumblue + (divisor / 2)) / divisor;
            newimage[i][j].rgbtGreen = (avesumgreen + (divisor / 2)) / divisor;
            newimage[i][j].rgbtRed = (avesumred + (divisor / 2)) / divisor;
        }
    }
    // move copy to the original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = newimage[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // more or less the same as blur but different calculations (Gx Gy)
    RGBTRIPLE newimage[height][width];
    // Create Gx and Gy I can reuse each time. never changes.
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // initialze the variables I'll need
            int sumbluex = 0;
            int sumgreenx = 0;
            int sumredx = 0;
            int sumbluey = 0;
            int sumgreeny = 0;
            int sumredy = 0;
            int ilo = -1;
            int ihi = 1;
            int jlo = -1;
            int jhi = 1;
            int edge = 0;
            // again, eliminate pixels we don't need to be calculating
            if (i == 0)
            {
                ilo = 0;
            }
            if (i == height - 1)
            {
                ihi = 0;
            }
            if (j == 0)
            {
                jlo = 0;
            }
            if (j == width - 1)
            {
                jhi = 0;
            }

            for (int k = ilo; k <= ihi; k++)
            {
                for (int l = jlo; l <= jhi; l++)
                {
                    // sum up Gx and Gy values for each color
                    sumbluex += Gx[k + 1][l + 1] * image[i + k][j + l].rgbtBlue;
                    sumgreenx += Gx[k + 1][l + 1] * image[i + k][j + l].rgbtGreen;
                    sumredx += Gx[k + 1][l + 1] * image[i + k][j + l].rgbtRed;
                    sumbluey += Gy[k + 1][l + 1] * image[i + k][j + l].rgbtBlue;
                    sumgreeny += Gy[k + 1][l + 1] * image[i + k][j + l].rgbtGreen;
                    sumredy += Gy[k + 1][l + 1] * image[i + k][j + l].rgbtRed;
                }
            }
            // get the uncapped value
            float Gzblue = round(sqrt((sumbluex * sumbluex) + (sumbluey * sumbluey)));
            float Gzgreen = round(sqrt((sumgreenx * sumgreenx) + (sumgreeny * sumgreeny)));
            float Gzred = round(sqrt((sumredx * sumredx) + (sumredy * sumredy)));
            // cap it
            if (Gzblue > 255)
            {
                Gzblue = 255;
            }
            if (Gzgreen > 255)
            {
                Gzgreen = 255;
            }
            if (Gzred > 255)
            {
                Gzred = 255;
            }
            // save to separate image
            newimage[i][j].rgbtBlue = Gzblue;
            newimage[i][j].rgbtGreen = Gzgreen;
            newimage[i][j].rgbtRed = Gzred;
        }
    }
    // move it on over
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = newimage[i][j];
        }
    }
    return;
}
