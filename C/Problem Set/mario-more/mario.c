#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int num;
    do
    {
        num = get_int("Height: ");
    }
    while (num < 1 || num > 8);

    for (int i = 1; i <= num; i++)
    {
        // Print spaces at the beginning
        for (int j = 0; j < num - i; j++)
        {
            printf(" ");
        }

        // Print the '#' for the left side
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }

        // Print spaces between both sides
        printf("  ");

        // Print the '#' for the right side
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }

        // Switch to the next line
        printf("\n");
    }
}
