#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height, i, j;
    // Get user input for size of pyramid, looping until an appropriate input is given
    do
    {
        height = get_int("Height: \n");
    }
    while (height > 8 || height < 1);
    // Loops on loops to build the pyramid, one large cycle per row
    // Starts at the top
    for (j = 1; j < height + 1; j++)
    {
        // First section of empty space
        for (i = 0; i < height - j; i++)
        {
            printf(" ");
        }
        // Pre gap blocks
        for (i = height; i > height - j; i--)
        {
            printf("#");
        }
        // Make the gap, no loop reqd
        printf("  ");
        // print second half of blocks
        for (i = height; i > height - j; i--)
        {
            printf("#");
        }
        printf("\n");
    }
}
