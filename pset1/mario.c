#include <cs50.h>
#include <stdio.h>

int main(void)
{   
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8); //Check input number gor height
    for (int strings = 0; strings < height; strings++) //Draw strings
    {
        for (int col = 0; col < height - strings - 1; col++) //Draw space before
        {
            printf(" ");
        }
        for (int col = 0; col < strings + 1; col++) //Draf first line of blocks
        {
            printf("#");
        }
        printf("  "); //Draw space between lines of blocks
        for (int col = 0; col < strings + 1; col++) //Draw second line of blocks
        {
            printf("#");
        }
        printf("\n");
    }
}
