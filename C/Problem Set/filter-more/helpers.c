#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int average;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average = round(((float) image[i][j].rgbtBlue + (float) image[i][j].rgbtRed + (float) image[i][j].rgbtGreen) / 3);
            image[i][j].rgbtRed = average;
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int half = width / 2;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < half; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blurImg[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = 0, green = 0, blue = 0, count = 0;
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    int first = i + x;
                    int second = j + y;

                    if (first >= 0 && first < height && second >= 0 && second < width)
                    {
                        red += image[first][second].rgbtRed;
                        green += image[first][second].rgbtGreen;
                        blue += image[first][second].rgbtBlue;
                        count++;
                    }
                }
            }
            blurImg[i][j].rgbtRed = round((float) red / count);
            blurImg[i][j].rgbtGreen = round((float) green / count);
            blurImg[i][j].rgbtBlue = round((float) blue / count);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blurImg[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int gxWeights[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gyWeights[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    RGBTRIPLE edgeImg[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int redX = 0, greenX = 0, blueX = 0, redY = 0, greenY = 0, blueY = 0, count = 0;
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    int gx = j + x;
                    int gy = i + y;
                    if (gx >= 0 && gx < width && gy >= 0 && gy < height)
                    {
                        redX += round(image[gy][gx].rgbtRed * gxWeights[y + 1][x + 1]);
                        greenX += round(image[gy][gx].rgbtGreen * gxWeights[y + 1][x + 1]);
                        blueX += round(image[gy][gx].rgbtBlue * gxWeights[y + 1][x + 1]);

                        redY += round(image[gy][gx].rgbtRed * gyWeights[y + 1][x + 1]);
                        greenY += round(image[gy][gx].rgbtGreen * gyWeights[y + 1][x + 1]);
                        blueY += round(image[gy][gx].rgbtBlue * gyWeights[y + 1][x + 1]);
                        count++;
                    }
                }
            }
            int newRed = round(sqrt(redX * redX + redY * redY)) > 255 ? 255 : round(sqrt(redX * redX + redY * redY));
            int newGreen =
                round(sqrt(greenX * greenX + greenY * greenY)) > 255 ? 255 : round(sqrt(greenX * greenX + greenY * greenY));
            int newBlue = round(sqrt(blueX * blueX + blueY * blueY)) > 255 ? 255 : round(sqrt(blueX * blueX + blueY * blueY));

            edgeImg[i][j].rgbtRed = newRed;
            edgeImg[i][j].rgbtGreen = newGreen;
            edgeImg[i][j].rgbtBlue = newBlue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = edgeImg[i][j];
        }
    }
    return;
}
