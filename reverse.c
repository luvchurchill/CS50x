#include <cs50.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wav.h"

// I used the cs50 duck debugger a fair amount for this project
// I hope that's ok. I never asked it for straight out solutions
// but I needed some help with malloc, fread, fwrite and free.
// it was also hard to debug because the output is binary...

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading, added safety feature
    char *input = argv[1];
    FILE *input_file = fopen(input, "r");
    if (input_file == NULL)
    {
        fclose(input_file);
        printf("File could not be opened, sorry");
        return 1;
    }

    // Read header
    WAVHEADER header;
    fread(&header, sizeof(WAVHEADER), 1, input_file);

    // Use check_format to ensure WAV format
    check_format(header);

    // Open output file for writing
    // Done! with extra safety
    char *output = argv[2];
    FILE *output_file = fopen(output, "wb");
    if (output_file == NULL)
    {
        fclose(input_file);
        printf("could not create %s\n", output);
        return 1;
    }

    // Write header to file
    // Done!
    fwrite(&header, sizeof(WAVHEADER), 1, output_file);

    // Use get_block_size to calculate size of block
    int block_size = get_block_size(header);

    // Write reversed audio to file
    int size_of_audio = header.subchunk2Size / block_size;
    unsigned char *audio = malloc(size_of_audio * sizeof(BYTE));
    if (audio == NULL)
    {
        printf("bad malloc\n");
        return 2;
    }
    fseek(input_file, (-1 * block_size), SEEK_END);
    int offset = -2 * block_size;
    for (int i = 0; i < size_of_audio; i++)
    {
        fread(audio, block_size, 1, input_file);
        fwrite(audio, block_size, 1, output_file);
        fseek(input_file, offset, SEEK_CUR);
    }
    fclose(input_file);
    fclose(output_file);
    free(audio);
}

int check_format(WAVHEADER header)
{
    // TODO #4
    char *wave = "WAVE";
    for (int i = 0; i < 4; i++)
    {
        if (header.format[i] != wave[i])
        {
            printf("not a WAV file\n");
            return false;
        }
    }
    return true;
}

int get_block_size(WAVHEADER header)
{
    // block size =  number of channels multiplied by bytes per sample
    int bytes_per_sample = header.bitsPerSample / 8;
    int block_size = header.numChannels * bytes_per_sample;
    return block_size;
}