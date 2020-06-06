#include <stdio.h>

int main(int argc, char *argv[])
{
    // Check usage
    if (argc != 2)
    {
        return 1;
    }

    // Open file
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        return 1;
    }

    // Read first three bytes
    unsigned char bytes[3];
    for (int i = 0; i < 5000; i++)
    {
        fseek(file, i, 0);
        fread(bytes, 3, 1, file);
        if (bytes[0] == 0xff)
        {
            printf("Maybe: %i\n", i);
        }
    }

    // Close file
    fclose(file);
}