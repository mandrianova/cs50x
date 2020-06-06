#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE middle;
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            middle = round(((float) image[h][w].rgbtBlue + (float) image[h][w].rgbtGreen + (float) image[h][w].rgbtRed) / 3);
            image[h][w].rgbtBlue = middle;
            image[h][w].rgbtGreen = middle;
            image[h][w].rgbtRed = middle;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    //create array for making sepia color for RGB
    int sepia[3];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            sepia[0] = round(.393 * (float) image[h][w].rgbtRed + .769 * (float) image[h][w].rgbtGreen + .189 * (float) image[h][w].rgbtBlue);
            sepia[1] = round(.349 * (float) image[h][w].rgbtRed + .686 * (float) image[h][w].rgbtGreen + .168 * (float) image[h][w].rgbtBlue);
            sepia[2] = round(.272 * (float) image[h][w].rgbtRed + .534 * (float) image[h][w].rgbtGreen + .131 * (float) image[h][w].rgbtBlue);
            //check sepia color
            for (int i = 0; i < 3; i++)
            {
                if (sepia[i] > 255)
                {
                    sepia[i] = 255;
                }
            }
            image[h][w].rgbtBlue = sepia[2];
            image[h][w].rgbtGreen = sepia[1];
            image[h][w].rgbtRed = sepia[0];
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //create tmp variable for moving pixel
    BYTE tmpBlue;
    BYTE tmpGreen;
    BYTE tmpRed;
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width / 2; w++) // move some pixels from one half of an image on to other half
        {
            // save current pixel in tmp
            tmpBlue = image[h][w].rgbtBlue;
            tmpGreen = image[h][w].rgbtGreen;
            tmpRed = image[h][w].rgbtRed;
            // copy in current pixel value from pixel of the other image's side
            image[h][w].rgbtBlue = image[h][width - w - 1].rgbtBlue;
            image[h][w].rgbtGreen = image[h][width - w - 1].rgbtGreen;
            image[h][w].rgbtRed = image[h][width - w - 1].rgbtRed;
            // copy in pixel from the other image's side value from tmp
            image[h][width - w - 1].rgbtBlue = tmpBlue;
            image[h][width - w - 1].rgbtGreen = tmpGreen;
            image[h][width - w - 1].rgbtRed = tmpRed;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //create copy of image for source color for bluring
    RGBTRIPLE copy_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy_image[i][j].rgbtBlue = image[i][j].rgbtBlue;
            copy_image[i][j].rgbtGreen = image[i][j].rgbtGreen;
            copy_image[i][j].rgbtRed = image[i][j].rgbtRed;
        }
    }
    //create tmp for sum of closer pixels
    int tmp[3];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            //start each loop with tmp = 0
            tmp[0] = 0;
            tmp[1] = 0;
            tmp[2] = 0;
            // create base variable for searching closer pixels in different situations
            int ch = -1, cw = -1, ch_lim = 2, cw_lim = 2;
            int n = 0; //count of closer pixels
            // take different cases of pixel's place
            if (h == 0) //first edge
            {
                ch = 0;
            }
            if (h == height - 1) //second edge
            {
                ch_lim = 1;
            }
            if (w == 0) //third edge
            {
                cw = 0;
            }
            if (w == width - 1) //fourth edge
            {
                cw_lim = 1;
            }
            for (int i = ch; i < ch_lim; i++) // make sum of closer pixels and count them
            {
                for (int j = cw; j < cw_lim; j++)
                {
                    tmp[0] += (int) copy_image[i + h][j + w].rgbtBlue;
                    tmp[1] += (int) copy_image[i + h][j + w].rgbtGreen;
                    tmp[2] += (int) copy_image[i + h][j + w].rgbtRed;
                    n++;
                }
            }
            image[h][w].rgbtBlue = round((float) tmp[0] / n);
            image[h][w].rgbtGreen = round((float) tmp[1] / n);
            image[h][w].rgbtRed = round((float) tmp[2] / n);
        }
    }
    return;
}

