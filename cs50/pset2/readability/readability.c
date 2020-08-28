#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    int count = 0; 
    // Collect input from the user
    printf("Text: ");
    string text = get_string("");
    // Calculate index
    double L = count_letters(text) / (count_words(text) / 100.0);
    double S = count_sentences(text) / (count_words(text) / 100.0);
    int index = 0.0588 * L - 0.296 * S - 15.8 + 0.5;
    // Checks for what should be printed
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 15)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}
// Count letters in a string
int count_letters(string text)
{
    int lcount = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // Detects any time an alphabetic character is used, then adds one to the letter count
        bool islet = isalpha(text[i]);
        if (islet)
        {
            lcount++;
        }
    }
    return lcount;
}
// Count words in a string
int count_words(string text)
{
    int wcount = 1;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // Detects any time a space is used, then adds one to the word count
        bool isword = isblank(text[i]);
        if (isword)
        {
            wcount++;
        }
    }
    return wcount;
}
// Count sentences in a string
int count_sentences(string text)
{
    int scount = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // Detects any time a '.', '!', or '?' is used and adds one the the sentence count
        bool issent = (text[i] == '.' || text[i] == '!' || text[i] == '?');
        if (issent)
        {
            scount++;
        }
    }
    return scount;
}