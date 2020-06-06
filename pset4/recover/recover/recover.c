#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Need a file");
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        return 1;
    }
    //create base variable
    int count_jpeg = 0; //for count of jpg
    int pointer = 0; // for pointer
    unsigned char bytes[512];
    char *file_name = malloc(9);
    // read file and create jpg
    while (!feof(file)) //untill not the end of file
    {
        fseek(file, pointer, 0); //move pointer for reading
        fread(bytes, 512, 1, file);
        // check that it is begining of jpg
        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && bytes[3] >= 0xe0 && bytes[3] <= 0xef)
        {
            sprintf(file_name, "%03i.jpg", count_jpeg);
            count_jpeg++;
        }
        if (count_jpeg > 0 && !feof(file)) // check that can create or open file and it is not end yet
        {
            FILE *recover_jpeg = fopen(file_name, "a");
            fwrite(bytes, 512, 1, recover_jpeg);
            fclose(recover_jpeg);
        }
        pointer += 512;
    }
    free(file_name);
    fclose(file);
    return 0;
}
