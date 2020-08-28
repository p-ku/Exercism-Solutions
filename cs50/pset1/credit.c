#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long ccnum = get_long("Number: ");
    long check = ccnum;
    int count = 0, sum = 0, p[33];
// Find how many digits and what they are, store in array
    do
    {
        p[count] = check % 10;
        check = (check - p[count]) / 10;
        count++;
    }
    while (check != 0);
// Run checksum
// Starting with first digit and skipping up
    for (int j = 0; j < count; j = j + 2)
    {
        sum = sum + p[j];
    }
// Then for the second digit and skipping up, this time double it
    for (int j = 1; j < count + 1; j = j + 2)
    {
        if (2 * p[j] > 9)
        {
            // This considers the value of the doubled digit's digits added
            sum = sum + (2 * p[j]) - 9;
        }
        else
        {
            sum = sum + 2 * p[j];
        }
    }
// If checksum passes, see if a card matches and identify
    if (sum % 10 == 0)
    {
        if (p[count - 1] == 4 && (count == 16 || count == 13))
        {
            printf("VISA\n");
        }
        else if (count == 15 && p[count - 1] == 3 && (p[count - 2] == 4 || p[count - 2] == 7))
        {
            printf("AMEX\n");
        }
        else if (count == 16 && p[count - 1] == 5 && (p[count - 2] == 1 || p[count - 2] == 2 || p[count - 2] == 3 || p[count - 2] == 4
                 || p[count - 2] == 5))
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
