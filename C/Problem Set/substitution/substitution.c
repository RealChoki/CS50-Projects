#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    // Check amount of command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // Check the length of the key
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters\n");
        return 1;
    }

    for (int i = 0, len = strlen(argv[1]); i < len; i++)
    {
        // Check if the character is not a letter
        if (!isupper(argv[1][i]) && !islower(argv[1][i]))
        {
            printf("Key must contain 26 letters\n");
            return 1;
        }
        // Check for repeated letters in the key
        for (int j = i + 1; j < len; j++)
        {
            if (tolower(argv[1][i]) == tolower(argv[1][j]))
            {
                printf("Key cannot contain the same letter twice\n");
                return 1;
            }
        }
    }

    // Get user input plaintext
    string input = get_string("plaintext: ");
    printf("ciphertext: ");

    // Encrypt the plaintext using the key
    for (int i = 0, len = strlen(input); i < len; i++)
    {
        if (isupper(input[i]))
        {
            // Encrypt uppercase letters
            printf("%c", toupper(argv[1][input[i] - 'A']));
        }

        else if (islower(input[i]))
        {
            // Encrypt lowercase letters
            int letterIndex = input[i] - 'A' - 32;
            printf("%c", tolower(argv[1][letterIndex]));
        }
        else
        {
            // Print non-alphabetic characters without changing
            printf("%c", input[i]);
        }
    }
    printf("\n");
    return 0;
}
