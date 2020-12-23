#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Compute grayscale value
            float red = image[i][j].rgbtRed;
            float green = image[i][j].rgbtGreen;
            float blue = image[i][j].rgbtBlue;
            int value = round((red + green + blue) / 3);

            // Replace pixels in image
            image[i][j].rgbtRed = value;
            image[i][j].rgbtGreen = value;
            image[i][j].rgbtBlue = value;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Create temporary arrays for color pigments
    int image_red[height][width];
    int image_blue[height][width];
    int image_green[height][width];

    // Fill arrays with reflected values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_red[i][width - 1 - j] = image[i][j].rgbtRed;
            image_blue[i][width - 1 - j] = image[i][j].rgbtBlue;
            image_green[i][width - 1 - j] = image[i][j].rgbtGreen;
        }
    }

    // Replace pixels in original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = image_red[i][j];
            image[i][j].rgbtGreen = image_green[i][j];
            image[i][j].rgbtBlue = image_blue[i][j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create temporary arrays for color pigments
    float image_red[height][width];
    float image_blue[height][width];
    float image_green[height][width];

    // Fill arrays with blurred values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Corners
            // Upper-left corner
            if (i == 0 && j == 0)
            {
                image_red[i][j] = round((2 + image[i][j].rgbtRed + image[i + 1][j].rgbtRed + image[i][j + 1].rgbtRed +
                                         image[i + 1][j + 1].rgbtRed) / 4);
                image_blue[i][j] = round((2 + image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i][j + 1].rgbtBlue +
                                          image[i + 1][j + 1].rgbtBlue) / 4);
                image_green[i][j] = round((2 + image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i][j + 1].rgbtGreen +
                                           image[i + 1][j + 1].rgbtGreen) / 4);
            }
            // Upper-right corner
            else if (i == 0 && j == width - 1)
            {
                image_red[i][j] = round((2 + image[i][j].rgbtRed + image[i + 1][j].rgbtRed + image[i][j - 1].rgbtRed +
                                         image[i + 1][j - 1].rgbtRed) / 4);
                image_blue[i][j] = round((2 + image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i][j - 1].rgbtBlue +
                                          image[i + 1][j - 1].rgbtBlue) / 4);
                image_green[i][j] = round((2 + image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i][j - 1].rgbtGreen +
                                           image[i + 1][j - 1].rgbtGreen) / 4);
            }
            // Lower-left corner
            else if (i == height - 1 && j == 0)
            {
                image_red[i][j] = round((2 + image[i][j].rgbtRed + image[i - 1][j].rgbtRed + image[i][j + 1].rgbtRed +
                                         image[i - 1][j + 1].rgbtRed) / 4);
                image_blue[i][j] = round((2 + image[i][j].rgbtBlue + image[i - 1][j].rgbtBlue + image[i][j + 1].rgbtBlue +
                                          image[i - 1][j + 1].rgbtBlue) / 4);
                image_green[i][j] = round((2 + image[i][j].rgbtGreen + image[i - 1][j].rgbtGreen + image[i][j + 1].rgbtGreen +
                                           image[i - 1][j + 1].rgbtGreen) / 4);
            }
            // Lower-right corner
            else if (i == height - 1 && j == width - 1)
            {
                image_red[i][j] = round((2 + image[i][j].rgbtRed + image[i - 1][j].rgbtRed + image[i][j - 1].rgbtRed +
                                         image[i - 1][j - 1].rgbtRed) / 4);
                image_blue[i][j] = round((2 + image[i][j].rgbtBlue + image[i - 1][j].rgbtBlue + image[i][j - 1].rgbtBlue +
                                          image[i - 1][j - 1].rgbtBlue) / 4);
                image_green[i][j] = round((2 + image[i][j].rgbtGreen + image[i - 1][j].rgbtGreen + image[i][j - 1].rgbtGreen +
                                           image[i - 1][j - 1].rgbtGreen) / 4);
            }

            // Edges
            // Upper edge
            else if (i == 0)
            {
                image_red[i][j] = round((3 + image[i][j - 1].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i][j].rgbtRed +
                                         image[i + 1][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed) / 6);
                image_blue[i][j] = round((3 + image[i][j - 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i][j].rgbtBlue +
                                          image[i + 1][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 6);
                image_green[i][j] = round((3 + image[i][j - 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i][j].rgbtGreen +
                                           image[i + 1][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 6);
            }
            // Lower edge
            else if (i == height - 1)
            {
                image_red[i][j] = round((3 + image[i][j - 1].rgbtRed + image[i - 1][j - 1].rgbtRed + image[i][j].rgbtRed +
                                         image[i - 1][j].rgbtRed + image[i][j + 1].rgbtRed + image[i - 1][j + 1].rgbtRed) / 6);
                image_blue[i][j] = round((3 + image[i][j - 1].rgbtBlue + image[i - 1][j - 1].rgbtBlue + image[i][j].rgbtBlue +
                                          image[i - 1][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i - 1][j + 1].rgbtBlue) / 6);
                image_green[i][j] = round((3 + image[i][j - 1].rgbtGreen + image[i - 1][j - 1].rgbtGreen + image[i][j].rgbtGreen +
                                           image[i - 1][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i - 1][j + 1].rgbtGreen) / 6);
            }
            // Left edge
            else if (j == 0)
            {
                image_red[i][j] = round((3 + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed + image[i][j].rgbtRed +
                                         image[i - 1][j].rgbtRed + image[i][j + 1].rgbtRed + image[i - 1][j + 1].rgbtRed) / 6);
                image_blue[i][j] = round((3 + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue + image[i][j].rgbtBlue +
                                          image[i - 1][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i - 1][j + 1].rgbtBlue) / 6);
                image_green[i][j] = round((3 + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen + image[i][j].rgbtGreen +
                                           image[i - 1][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i - 1][j + 1].rgbtGreen) / 6);
            }
            // Right edge
            else if (j == width - 1)
            {
                image_red[i][j] = round((3 + image[i + 1][j].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i][j].rgbtRed +
                                         image[i - 1][j].rgbtRed + image[i][j - 1].rgbtRed + image[i - 1][j - 1].rgbtRed) / 6);
                image_blue[i][j] = round((3 + image[i + 1][j].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i][j].rgbtBlue +
                                          image[i - 1][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i - 1][j - 1].rgbtBlue) / 6);
                image_green[i][j] = round((3 + image[i + 1][j].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i][j].rgbtGreen +
                                           image[i - 1][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i - 1][j - 1].rgbtGreen) / 6);
            }

            // Centerpoints
            else
            {
                image_red[i][j] = round((4 + image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed +
                                         image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed +
                                         image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) / 9);
                image_blue[i][j] = round((4 + image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue +
                                          image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue +
                                          image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 9);
                image_green[i][j] = round((4 + image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen +
                                           image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen +
                                           image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 9);
            }
        }
    }

    // Replace pixels in original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = image_red[i][j];
            image[i][j].rgbtGreen = image_green[i][j];
            image[i][j].rgbtBlue = image_blue[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create temporary arrays for color pigments
    float image_red_gx[height][width];
    float image_blue_gx[height][width];
    float image_green_gx[height][width];

    float image_red_gy[height][width];
    float image_blue_gy[height][width];
    float image_green_gy[height][width];

    // Loop through all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Corners
            // Upper-left corner
            if (i == 0 && j == 0)
            {
                image_red_gx[i][j] = 0 * image[i][j].rgbtRed + 0 * image[i][j + 1].rgbtRed + 
                                     2 * image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed;
                image_blue_gx[i][j] = 0 * image[i][j].rgbtBlue + 0 * image[i][j + 1].rgbtBlue + 
                                      2 * image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue;
                image_green_gx[i][j] = 0 * image[i][j].rgbtGreen + 0 * image[i][j + 1].rgbtGreen + 
                                       2 * image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen;

                image_red_gy[i][j] = 0 * image[i][j].rgbtRed + 2 * image[i][j + 1].rgbtRed + 
                                     0 * image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed;
                image_blue_gy[i][j] = 0 * image[i][j].rgbtBlue + 2 * image[i][j + 1].rgbtBlue + 
                                      0 * image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue;
                image_green_gy[i][j] = 0 * image[i][j].rgbtGreen + 2 * image[i][j + 1].rgbtGreen + 
                                       0 * image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen;
            }
            // Upper-right corner
            else if (i == 0 && j == width - 1)
            {
                image_red_gx[i][j] = 0 * image[i][j - 1].rgbtRed + 0 * image[i][j].rgbtRed + 
                                     image[i + 1][j - 1].rgbtRed + 2 * image[i + 1][j].rgbtRed;
                image_blue_gx[i][j] = 0 * image[i][j - 1].rgbtBlue + 0 * image[i][j].rgbtBlue + 
                                      image[i + 1][j - 1].rgbtBlue + 2 * image[i + 1][j].rgbtBlue;
                image_green_gx[i][j] = 0 * image[i][j - 1].rgbtGreen + 0 * image[i][j].rgbtGreen + 
                                       image[i + 1][j - 1].rgbtGreen + 2 * image[i + 1][j].rgbtGreen;

                image_red_gy[i][j] = -2 * image[i][j - 1].rgbtRed + 0 * image[i][j].rgbtRed - 
                                     image[i + 1][j - 1].rgbtRed + 0 * image[i + 1][j].rgbtRed;
                image_blue_gy[i][j] = -2 * image[i][j - 1].rgbtBlue + 0 * image[i][j].rgbtBlue - 
                                      image[i + 1][j - 1].rgbtBlue + 0 * image[i + 1][j].rgbtBlue;
                image_green_gy[i][j] = -2 * image[i][j - 1].rgbtGreen + 0 * image[i][j].rgbtGreen - 
                                       image[i + 1][j - 1].rgbtGreen + 0 * image[i + 1][j].rgbtGreen;
            }
            // Lower-left corner
            else if (i == height - 1 && j == 0)
            {
                image_red_gx[i][j] = -2 * image[i - 1][j].rgbtRed - image[i - 1][j + 1].rgbtRed + 
                                     0 * image[i][j].rgbtRed + 0 * image[i][j + 1].rgbtRed;
                image_blue_gx[i][j] = -2 * image[i - 1][j].rgbtBlue - image[i - 1][j + 1].rgbtBlue + 
                                      0 * image[i][j].rgbtBlue + 0 * image[i][j + 1].rgbtBlue;
                image_green_gx[i][j] = -2 * image[i - 1][j].rgbtGreen - image[i - 1][j + 1].rgbtGreen + 
                                       0 * image[i][j].rgbtGreen + 0 * image[i][j + 1].rgbtGreen;

                image_red_gy[i][j] = -0 * image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + 
                                     0 * image[i][j].rgbtRed + 2 * image[i][j + 1].rgbtRed;
                image_blue_gy[i][j] = -0 * image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + 
                                      0 * image[i][j].rgbtBlue + 2 * image[i][j + 1].rgbtBlue;
                image_green_gy[i][j] = -0 * image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + 
                                       0 * image[i][j].rgbtGreen + 2 * image[i][j + 1].rgbtGreen;
            }
            // Lower-right corner
            else if (i == height - 1 && j == width - 1)
            {
                image_red_gx[i][j] = -image[i - 1][j - 1].rgbtRed - 2 * image[i - 1][j].rgbtRed  + 
                                     0 * image[i][j - 1].rgbtRed + 0 * image[i][j].rgbtRed;
                image_blue_gx[i][j] = -image[i - 1][j - 1].rgbtBlue - 2 * image[i - 1][j].rgbtBlue + 
                                      0 * image[i][j - 1].rgbtBlue + 0 * image[i][j].rgbtBlue;
                image_green_gx[i][j] = -image[i - 1][j - 1].rgbtGreen - 2 * image[i - 1][j].rgbtGreen + 
                                       0 * image[i][j - 1].rgbtGreen + 0 * image[i][j].rgbtGreen;

                image_red_gy[i][j] = -image[i - 1][j - 1].rgbtRed - 0 * image[i - 1][j].rgbtRed - 
                                     2 * image[i][j - 1].rgbtRed + 0 * image[i][j].rgbtRed;
                image_blue_gy[i][j] = -image[i - 1][j - 1].rgbtBlue - 0 * image[i - 1][j].rgbtBlue - 
                                      2 * image[i][j - 1].rgbtBlue + 0 * image[i][j].rgbtBlue;
                image_green_gy[i][j] = -image[i - 1][j - 1].rgbtGreen - 0 * image[i - 1][j].rgbtGreen - 
                                       2 * image[i][j - 1].rgbtGreen + 0 * image[i][j].rgbtGreen;
            }
            
            // Edges
            // Upper edge
            else if (i == 0)
            {
                image_red_gx[i][j] = 0 * image[i][j - 1].rgbtRed + 0 * image[i][j].rgbtRed + 0 * image[i][j + 1].rgbtRed +
                                     image[i + 1][j - 1].rgbtRed + 2 * image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed;
                image_blue_gx[i][j] = 0 * image[i][j - 1].rgbtBlue + 0 * image[i][j].rgbtBlue + 0 * image[i][j + 1].rgbtBlue +
                                      image[i + 1][j - 1].rgbtBlue + 2 * image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue;
                image_green_gx[i][j] = 0 * image[i][j - 1].rgbtGreen + 0 * image[i][j].rgbtGreen + 0 * image[i][j + 1].rgbtGreen +
                                       image[i + 1][j - 1].rgbtGreen + 2 * image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen;

                image_red_gy[i][j] = -2 * image[i][j - 1].rgbtRed + 0 * image[i][j].rgbtRed + 2 * image[i][j + 1].rgbtRed -
                                     image[i + 1][j - 1].rgbtRed + 0 * image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed;
                image_blue_gy[i][j] = -2 * image[i][j - 1].rgbtBlue + 0 * image[i][j].rgbtBlue + 2 * image[i][j + 1].rgbtBlue -
                                      image[i + 1][j - 1].rgbtBlue + 0 * image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue;
                image_green_gy[i][j] = -2 * image[i][j - 1].rgbtGreen + 0 * image[i][j].rgbtGreen + 2 * image[i][j + 1].rgbtGreen -
                                       image[i + 1][j - 1].rgbtGreen + 0 * image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen;
            }
            // Lower edge
            else if (i == height - 1)
            {
                image_red_gx[i][j] = -image[i - 1][j - 1].rgbtRed - 2 * image[i - 1][j].rgbtRed - image[i - 1][j + 1].rgbtRed +
                                     0 * image[i][j - 1].rgbtRed + 0 * image[i][j].rgbtRed + 0 * image[i][j + 1].rgbtRed;
                image_blue_gx[i][j] = -image[i - 1][j - 1].rgbtBlue - 2 * image[i - 1][j].rgbtBlue - image[i - 1][j + 1].rgbtBlue +
                                      0 * image[i][j - 1].rgbtBlue + 0 * image[i][j].rgbtBlue + 0 * image[i][j + 1].rgbtBlue;
                image_green_gx[i][j] = -image[i - 1][j - 1].rgbtGreen - 2 * image[i - 1][j].rgbtGreen - image[i - 1][j + 1].rgbtGreen +
                                       0 * image[i][j - 1].rgbtGreen + 0 * image[i][j].rgbtGreen + 0 * image[i][j + 1].rgbtGreen;

                image_red_gy[i][j] = -image[i - 1][j - 1].rgbtRed - 0 * image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed -
                                     2 * image[i][j - 1].rgbtRed + 0 * image[i][j].rgbtRed + 2 * image[i][j + 1].rgbtRed;
                image_blue_gy[i][j] = -image[i - 1][j - 1].rgbtBlue - 0 * image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue -
                                      2 * image[i][j - 1].rgbtBlue + 0 * image[i][j].rgbtBlue + 2 * image[i][j + 1].rgbtBlue;
                image_green_gy[i][j] = -image[i - 1][j - 1].rgbtGreen - 0 * image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen -
                                       2 * image[i][j - 1].rgbtGreen + 0 * image[i][j].rgbtGreen + 2 * image[i][j + 1].rgbtGreen;
            }
            // Left edge
            else if (j == 0)
            {
                image_red_gx[i][j] = - 2 * image[i - 1][j].rgbtRed - image[i - 1][j + 1].rgbtRed +
                                     0 * image[i][j].rgbtRed + 0 * image[i][j + 1].rgbtRed +
                                     2 * image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed;
                image_blue_gx[i][j] = - 2 * image[i - 1][j].rgbtBlue - image[i - 1][j + 1].rgbtBlue +
                                      0 * image[i][j].rgbtBlue + 0 * image[i][j + 1].rgbtBlue +
                                      2 * image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue;
                image_green_gx[i][j] = - 2 * image[i - 1][j].rgbtGreen - image[i - 1][j + 1].rgbtGreen +
                                       0 * image[i][j].rgbtGreen + 0 * image[i][j + 1].rgbtGreen +
                                       2 * image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen;

                image_red_gy[i][j] = - 0 * image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed +
                                     0 * image[i][j].rgbtRed + 2 * image[i][j + 1].rgbtRed +
                                     0 * image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed;
                image_blue_gy[i][j] = - 0 * image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue +
                                      0 * image[i][j].rgbtBlue + 2 * image[i][j + 1].rgbtBlue +
                                      0 * image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue;
                image_green_gy[i][j] = - 0 * image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen +
                                       0 * image[i][j].rgbtGreen + 2 * image[i][j + 1].rgbtGreen +
                                       0 * image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen;
            }
            // Right edge
            else if (j == width - 1)
            {
                image_red_gx[i][j] = -image[i - 1][j - 1].rgbtRed - 2 * image[i - 1][j].rgbtRed +
                                     0 * image[i][j - 1].rgbtRed + 0 * image[i][j].rgbtRed +
                                     image[i + 1][j - 1].rgbtRed + 2 * image[i + 1][j].rgbtRed;
                image_blue_gx[i][j] = -image[i - 1][j - 1].rgbtBlue - 2 * image[i - 1][j].rgbtBlue +
                                      0 * image[i][j - 1].rgbtBlue + 0 * image[i][j].rgbtBlue +
                                      image[i + 1][j - 1].rgbtBlue + 2 * image[i + 1][j].rgbtBlue;
                image_green_gx[i][j] = -image[i - 1][j - 1].rgbtGreen - 2 * image[i - 1][j].rgbtGreen +
                                       0 * image[i][j - 1].rgbtGreen + 0 * image[i][j].rgbtGreen +
                                       image[i + 1][j - 1].rgbtGreen + 2 * image[i + 1][j].rgbtGreen;

                image_red_gy[i][j] = -image[i - 1][j - 1].rgbtRed - 0 * image[i - 1][j].rgbtRed -
                                     2 * image[i][j - 1].rgbtRed + 0 * image[i][j].rgbtRed -
                                     image[i + 1][j - 1].rgbtRed + 0 * image[i + 1][j].rgbtRed;
                image_blue_gy[i][j] = -image[i - 1][j - 1].rgbtBlue - 0 * image[i - 1][j].rgbtBlue -
                                      2 * image[i][j - 1].rgbtBlue + 0 * image[i][j].rgbtBlue -
                                      image[i + 1][j - 1].rgbtBlue + 0 * image[i + 1][j].rgbtBlue;
                image_green_gy[i][j] = -image[i - 1][j - 1].rgbtGreen - 0 * image[i - 1][j].rgbtGreen -
                                       2 * image[i][j - 1].rgbtGreen + 0 * image[i][j].rgbtGreen -
                                       image[i + 1][j - 1].rgbtGreen + 0 * image[i + 1][j].rgbtGreen;
            }
            
            // Centerpoints
            else
            {
                image_red_gx[i][j] = -image[i - 1][j - 1].rgbtRed - 2 * image[i - 1][j].rgbtRed - image[i - 1][j + 1].rgbtRed +
                                     0 * image[i][j - 1].rgbtRed + 0 * image[i][j].rgbtRed + 0 * image[i][j + 1].rgbtRed +
                                     image[i + 1][j - 1].rgbtRed + 2 * image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed;
                image_blue_gx[i][j] = -image[i - 1][j - 1].rgbtBlue - 2 * image[i - 1][j].rgbtBlue - image[i - 1][j + 1].rgbtBlue +
                                      0 * image[i][j - 1].rgbtBlue + 0 * image[i][j].rgbtBlue + 0 * image[i][j + 1].rgbtBlue +
                                      image[i + 1][j - 1].rgbtBlue + 2 * image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue;
                image_green_gx[i][j] = -image[i - 1][j - 1].rgbtGreen - 2 * image[i - 1][j].rgbtGreen - image[i - 1][j + 1].rgbtGreen +
                                       0 * image[i][j - 1].rgbtGreen + 0 * image[i][j].rgbtGreen + 0 * image[i][j + 1].rgbtGreen +
                                       image[i + 1][j - 1].rgbtGreen + 2 * image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen;

                image_red_gy[i][j] = -image[i - 1][j - 1].rgbtRed - 0 * image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed -
                                     2 * image[i][j - 1].rgbtRed + 0 * image[i][j].rgbtRed + 2 * image[i][j + 1].rgbtRed -
                                     image[i + 1][j - 1].rgbtRed + 0 * image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed;
                image_blue_gy[i][j] = -image[i - 1][j - 1].rgbtBlue - 0 * image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue -
                                      2 * image[i][j - 1].rgbtBlue + 0 * image[i][j].rgbtBlue + 2 * image[i][j + 1].rgbtBlue -
                                      image[i + 1][j - 1].rgbtBlue + 0 * image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue;
                image_green_gy[i][j] = -image[i - 1][j - 1].rgbtGreen - 0 * image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen -
                                       2 * image[i][j - 1].rgbtGreen + 0 * image[i][j].rgbtGreen + 2 * image[i][j + 1].rgbtGreen -
                                       image[i + 1][j - 1].rgbtGreen + 0 * image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen;
            }
        }
    }

    // Replace pixels in original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Compute new values to detect edges
            float red = 0.5 + sqrt(image_red_gx[i][j] * image_red_gx[i][j] + image_red_gy[i][j] * image_red_gy[i][j]);
            float green = 0.5 + sqrt(image_green_gx[i][j] * image_green_gx[i][j] + image_green_gy[i][j] * image_green_gy[i][j]);
            float blue = 0.5 + sqrt(image_blue_gx[i][j] * image_blue_gx[i][j] + image_blue_gy[i][j] * image_blue_gy[i][j]);
            
            // Bind values to 255
            if (red > 255)
            {
                red = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (blue > 255)
            {
                blue = 255;
            }
            
            // Replace image pixels with new values
            image[i][j].rgbtRed = red;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtBlue = blue;
        }
    }
    return;
}
