#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float avg = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0;
            image[i][j].rgbtRed = round(avg);
            image[i][j].rgbtGreen = round(avg);
            image[i][j].rgbtBlue = round(avg);
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int original_red = image[i][j].rgbtRed;
            int original_green = image[i][j].rgbtGreen;
            int original_blue = image[i][j].rgbtBlue;
            float sepia_red = .393 * original_red + .769 * original_green + .189 * original_blue;
            float sepia_green = .349 * original_red + .686 * original_green + .168 * original_blue;
            float sepia_blue = .272 * original_red + .534 * original_green + .131 * original_blue;
            // Can't be higher than 255
            if (round(sepia_red) > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = round(sepia_red);
            }
            if (round(sepia_green) > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = round(sepia_green);
            }
            if (round(sepia_blue) > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = round(sepia_blue);
            }
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (width % 2 == 1)
            {
                // This is ugly but it works and i'm scared to touch it
                int dist_from_middle = 0;
                int middle = width / 2;
                if (j > middle)
                {
                    dist_from_middle = j - round(middle);
                }
                if (j < middle)
                {
                    dist_from_middle = round(middle) - j;
                }
                RGBTRIPLE tmp = image[i][j];
                image[i][j] = image[i][middle - dist_from_middle];
                image[i][middle - dist_from_middle] = tmp;
            }
            else
            {
                // This is much nicer
                int oppo = width - j;
                float middle = width / 2;
                if (oppo > middle)
                {
                    RGBTRIPLE tmp = image[i][j];
                    image[i][j] = image[i][oppo - 1];
                    image[i][oppo - 1] = tmp;
                }
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy to work with
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    // Loop through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red_sum = 0;
            int green_sum = 0;
            int blue_sum = 0;
            int divisor = 0;
            // Loop through nearby pixels and check if they exist
            for (int di = i - 1; di <= i + 1; di++)
            {
                for (int dj = j - 1; dj <= j + 1; dj++)
                {
                    if (di >= 0 && dj >= 0 && di < height && dj < width)
                    {
                        red_sum += copy[di][dj].rgbtRed;
                        blue_sum += copy[di][dj].rgbtBlue;
                        green_sum += copy[di][dj].rgbtGreen;
                        divisor++;
                    }
                }
            }
            // Get the average and substitute it into the original image
            float red_avg_flt = (float) red_sum / divisor;
            float blue_avg_flt = (float) blue_sum / divisor;
            float green_avg_flt = (float) green_sum / divisor;
            int red_avg = round(red_avg_flt);
            int blue_avg = round(blue_avg_flt);
            int green_avg = round(green_avg_flt);
            image[i][j].rgbtRed = red_avg;
            image[i][j].rgbtBlue = blue_avg;
            image[i][j].rgbtGreen = green_avg;
        }
    }

    return;
}
