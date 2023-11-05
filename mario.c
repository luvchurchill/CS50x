#include <cs50.h>
#include <stdio.h>

int get_size(void);
int main(void)

{
    int size = get_size();
    int i = 0;
    while (i < size)

    {
        printf("\n");
        i++;
        int j = i;
        int k = i;
        while (j < size)

        {
            printf(" ");
            j++;
            if (j >= size)
            {
                while (k > 0)
                {
                    printf("#");
                    k--;
                }
            }
        }
    }
    int l = 0;
    while (l < size)
    {
        printf("#");
        l++;
    }
    printf("\n");
}

int get_size(void)
{
    int size;
    do
    {
        size = get_int("Size: ");
    }
    while (size > 8 || size < 1);
    return size;
}
