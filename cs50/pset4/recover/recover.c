#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // exit if no file as inline argument
    if (argc == 1)
    {
        return 1;
    }
    unsigned int count = 0;
    // create BYTE type
    typedef uint8_t BYTE;
    FILE *img;
    BYTE *buffer = malloc(512 * sizeof(BYTE));
    FILE *f = fopen(argv[1], "r");
    char *filename = malloc(8 * sizeof(char));
    unsigned int length;
    do
    {
        length = fread(buffer, 1, 512, f);
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // first image found is written here
            if (count == 0)
            {
                sprintf(filename, "%03i.jpg", count);
                img = fopen(filename, "a");
                length = fwrite(buffer, 1, length, img);
                count += 1;
            }
            // all other images are written here
            else
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", count);
                img = fopen(filename, "a");
                fwrite(buffer, 1, length, img);
                count += 1;
            }
        }
        // longer jpegs are written further here
        else
        {
            if (count == 0)
            {
            }
            else
            {
                fwrite(buffer, 1, length, img);
            }
        }
    }
    // check if length is a full 512 before continuing loop
    while (length == 512);
    // free up memory that was allocated
    free(buffer);
    free(filename);
    return 0;
}
