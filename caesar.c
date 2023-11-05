#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int digit(string arg);

int main(int argc, string argv[])
{
    if (argc != 2 || digit(argv[1]) == 1010)
    {
        printf("usage: ./caeser key\n");
        return 1;
    }
    else
    {
        string keystring = argv[1];
        int keyint = atoi(keystring);
        int key;
        if (keyint > 26)
        {
            key = keyint % 26;
        }
        else
        {
            key = keyint;
        }
        string alpha = "abcdefghijklmnopqrstuvwxyz";
        string input = get_string("Plaintext:  ");
        printf("ciphertext: ");
        for (int i = 0; i < strlen(input); i++)
            if (islower(input[i]))
            {
                printf("%c", alpha[(input[i] - 97 + key) % 26]);
            }
            else if (isupper(input[i]))
            {
                printf("%c", toupper(alpha[(input[i] - 65 + key) % 26]));
            }
            else
            {
                printf("%c", input[i]);
            }
        printf("\n");
        return 0;
    }
}

int digit(string arg)
{
    int result;
    for (int i = 0; i < strlen(arg); i++)
    {
        if (arg[i] < 48 || arg[i] > 57)
        {
            result = 1010;
        }
        else
        {
            result = 3;
        }
    }
    return result;
}
