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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //create copy of image for source color
    RGBTRIPLE copy_image[height + 2][width + 2];
    for (int i = 0; i < height + 2; i++)
    {
        for (int j = 0; j < width + 2; j++)
        {
            if (i != 0 && j != 0 && i != height + 1 && j != width + 1)
            {
                copy_image[i][j].rgbtBlue = image[i - 1][j - 1].rgbtBlue;
                copy_image[i][j].rgbtGreen = image[i - 1][j - 1].rgbtGreen;
                copy_image[i][j].rgbtRed = image[i - 1][j - 1].rgbtRed;
            }
            else
            {
                copy_image[i][j].rgbtBlue = 0;
                copy_image[i][j].rgbtGreen = 0;
                copy_image[i][j].rgbtRed = 0;
            }
        }
    }
    int gx[3];
    int gy[3];
    int tmp[3];
    int kx[3][3], ky[3][3]; // coefficients to calculate the color value of a close pixel
    for (int i = 0; i < 3; i++) // create coub for coefficients
    {
        for (int j = 0; j < 3; j++)
        {
            kx[i][j] = 1;
            ky[i][j] = 1;
            if (i == 0)
            {
                kx[i][j] = kx[i][j] * (-1);
            }
            if (i == 1)
            {
                kx[i][j] = kx[i][j] * 0;
                ky[i][j] = ky[i][j] * 2;
            }
            if (j == 0)
            {
                ky[i][j] = ky[i][j] * (-1);
            }
            if (j == 1)
            {
                kx[i][j] = kx[i][j] * 2;
                ky[i][j] = ky[i][j] * 0;
            }
        }
    }
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // reset variable
            gx[0] = 0;
            gx[1] = 0;
            gx[2] = 0;
            gy[0] = 0;
            gy[1] = 0;
            gy[2] = 0;
            //start calculate value of close pixels
            for (int i = -1; i < 2; i++) // make sum of closer pixels and count them
            {
                for (int j = -1; j < 2; j++)
                {
                    gx[0] += kx[i + 1][j + 1] * (int) copy_image[i + h + 1][j + w + 1].rgbtBlue;
                    gx[1] += kx[i + 1][j + 1] * (int) copy_image[i + h + 1][j + w + 1].rgbtGreen;
                    gx[2] += kx[i + 1][j + 1] * (int) copy_image[i + h + 1][j + w + 1].rgbtRed;
                    gy[0] += ky[i + 1][j + 1] * (int) copy_image[i + h + 1][j + w + 1].rgbtBlue;
                    gy[1] += ky[i + 1][j + 1] * (int) copy_image[i + h + 1][j + w + 1].rgbtGreen;
                    gy[2] += ky[i + 1][j + 1] * (int) copy_image[i + h + 1][j + w + 1].rgbtRed;
                }
            }
            //calculate sqtr gx^2 + gy^2
            tmp[0] =  round(sqrt(gx[0] * gx[0] + gy[0] * gy[0]));
            tmp[1] = round(sqrt(gx[1] * gx[1] + gy[1] * gy[1]));
            tmp[2] = round(sqrt(gx[2] * gx[2] + gy[2] * gy[2]));
            for (int i = 0; i < 3; i++) //check interval
            {
                if (tmp[i] > 255)
                {
                    tmp[i] = 255;
                }
            }
            image[h][w].rgbtBlue = tmp[0];
            image[h][w].rgbtGreen = tmp[1];
            image[h][w].rgbtRed = tmp[2];
        }
    }
    return;
}
