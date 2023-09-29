#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    // TODO #2
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("wrong file format\n");
        return 1;
    }

    // Read header
    // TODO #3
    WAVHEADER header;
    size_t readElements = fread(&header, sizeof(WAVHEADER), 1, file);
    if (readElements < 1)
    {
        printf("Problem with reading file\n");
        return 1;
    }

    // Use check_format to ensure WAV format
    // TODO #4
    if (check_format(header) == 0)
    {
        printf("Wrong file format\n");
        return 1;
    }

    // Open output file for writing
    // TODO #5
    FILE *outFile = fopen(argv[2], "w");
    if (outFile == NULL)
    {
        return 1;
    }

    // Write header to file
    // TODO #6
    size_t writeElements = fwrite(&header, sizeof(WAVHEADER), 1, outFile);
    if (writeElements < 1)
    {
        printf("Problem with writing to file\n");
        return 1;
    }

    // Use get_block_size to calculate size of block
    // TODO #7
    int block = get_block_size(header);

    // Write reversed audio to file
    // TODO #8
    if (fseek(file, block, SEEK_END))
    {
        return 1;
    }
    BYTE reversed[block];
    while (ftell(file) - block > sizeof(header))
    {
        if (fseek(file, -2 * block, SEEK_CUR))
        {
            return 1;
        }
        size_t items_read = fread(reversed, block, 1, file);
        if (items_read != 1)
        {
            printf("problem in reading\n");
            return 1;
        }
        size_t items_written = fwrite(reversed, block, 1, outFile);
        if (items_written != 1)
        {
            printf("problem in writing\n");
            return 1;
        }
    }

    fclose(file);
    fclose(outFile);
}

int check_format(WAVHEADER header)
{
    // TODO #4
    if (header.format[0] == 'W' && header.format[1] == 'A' && header.format[2] == 'V' && header.format[3] == 'E')
    {
        return 1;
    }
    return 0;
}

int get_block_size(WAVHEADER header)
{
    // TODO #7
    int block = header.numChannels * header.bitsPerSample / 8;
    return block;
}
