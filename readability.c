#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// I couldn't figure out how to round to nearest int so I searched online
// and found the round() function. everything else is my own work.

int letters(string text);
int words(string text);
int sentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    float amount_letters = letters(text);
    float amount_words = words(text);
    float amount_sentence = sentences(text);
    float L = (amount_letters / amount_words) * 100;
    float S = (amount_sentence / amount_words) * 100;
    float index = (0.0588 * L) - (0.296 * S) - 15.8;
    int rounded = round(index);
    if (rounded < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (rounded > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", rounded);
    }
}

int letters(string text)
{
    int letter_count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isupper(text[i]) || islower(text[i]))
        {
            letter_count += 1;
        }
        else
        {
            letter_count += 0;
        }
    }
    return letter_count;
}

int words(string text)
{
    int word_count = 1;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == 32)
        {
            word_count += 1;
        }
        else
        {
            word_count += 0;
        }
    }
    return word_count;
}

int sentences(string text)
{
    int sentence_count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == 46 || text[i] == 63 || text[i] == 33)
        {
            sentence_count += 1;
        }
        else
        {
            sentence_count += 0;
        }
    }
    return sentence_count;
}
