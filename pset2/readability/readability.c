#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);
    float L = (float) letters * 100 / (float) words; // the average number of letters per 100 words in the text
    float S = (float) sentences * 100 / (float) words; // the average number of sentences per 100 words in the text
    float index = 0.0588 * L - 0.296 * S - 15.8; // formula for Coleman-Liau index
    int grade = roundf(index);
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade <= 16)
    {
        printf("Grade %i\n", grade);
    }
    else
    {
        printf("Grade 16+\n");
    }
}

int count_letters(string text)
{
    int lenght = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z')) //count only letters
        {
            lenght++;
        }
    }
    return lenght;
}

int count_words(string text)
{
    int count = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == ' ') // count spaces between words
        {
            count++;
        }
    }
    return count + 1; //add 1 because after last word dosn't have any spaces
}

int count_sentences(string text)
{
    int count = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?') //count all '.', '?' and '!'
        {
            count++;
        }
    }
    return count;
}