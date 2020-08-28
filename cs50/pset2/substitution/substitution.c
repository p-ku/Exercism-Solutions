#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[])
{

    // VALIDATE KEY
    // Check key length
    if (argc == 2 && argv[1])
    {
        int keylen = strlen(argv[1]);
        if (keylen == 26)
        {
            // Check for non-alphabetic characters
            for (int i = 0; i < 26; i++)
            {
                if (isalpha(argv[1][i]))
                {
                    // Standardize characters such that all are uppercase
                    if (islower(argv[1][i]))
                    {
                        argv[1][i] -= 32;
                    }
                    // Check for repeated characters
                    for (int j = i - 1; j > 0; j--)
                    {
                        if (argv[1][i] != argv[1][j])
                        {
                        }
                        else
                        {
                            printf("Usage: ./substitution key\n");
                            return 1;
                        }
                    }
                }
                else
                {
                    printf("Usage: ./substitution key\n");
                    return 1;
                }
            }
            // Valid, continue process here
            // GET PLAINTEXT
            string plain = get_string("plaintext: ");
            // ENCIPHER
            int n = strlen(plain);
            char cipher[n];
            printf("ciphertext: ");
            int index;
            for (int i = 0; i < n; i++)
            {
                if (isupper(plain[i]))
                {
                    index = plain[i] - 65;
                    cipher[i] = argv[1][index];
                }
                else if (islower(plain[i]))
                {
                    index = plain[i] - 97;
                    cipher[i] = argv[1][index] + 32;
                }
                else
                {
                    cipher[i] = plain[i];
                }
                // PRINT CIPHERTEXT
                printf("%c", cipher[i]);
            }
            printf("\n");
            return 0;
        }
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
}