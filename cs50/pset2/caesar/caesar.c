#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h> //atoi

int main(int argc, string argv[])
{

    // VALIDATE KEY
    // Check key length
    if (argc == 2 && argv[1])
    {
        int keylen = strlen(argv[1]);
        for (int i = 0; i < keylen; i++)
        {
            if (!isdigit(argv[1][i]))
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }
        int k = atoi(argv[1]);
        printf("Success\n%i\n", k);
        // GET PLAINTEXT
        string plain = get_string("plaintext: ");
        int plen = strlen(plain);
        char cipher[plen];
        for (int i = 0; i < plen; i++)
        {
            int index;
            if(isupper(plain[i]))
            {
                index = plain[i] - 65;
            }
            else
            {
                index = plain[i] - 97;
            }
            cipher[i] = (index + k) % 26;
        }
        printf("ciphertext: %s\n", cipher);
    }
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}