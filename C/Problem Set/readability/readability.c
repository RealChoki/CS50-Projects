#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string input = get_string("Text: ");
    int lettersNum = count_letters(input);
    int wordsNum = count_words(input);
    int sentencesNum = count_sentences(input);

    float grade = 0.0588 * ((float) lettersNum / wordsNum * 100) - 0.296 * ((float) sentencesNum / wordsNum * 100) - 15.8;

    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(grade));
    }
}

// Counts and returns the amount of letters
int count_letters(string text)
{
    int n = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isupper(text[i]) || islower(text[i]))
        {
            n++;
        }
    }
    return n;
}

// Counts and returns the amount of words
int count_words(string text)
{
    int n = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (text[i] == ' ')
        {
            n++;
        }
    }
    return n + 1;
}

// Counts and returns the amount of sentences
int count_sentences(string text)
{
    int n = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            n++;
        }
    }
    return n;
}