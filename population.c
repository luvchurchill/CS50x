#include <cs50.h>
#include <stdio.h>

int main(void)

{
    int start_lama;
    do
    {
        start_lama = get_int("Start size: ");
    }
    while (start_lama < 9);

    int end_lama;
    do
    {
        end_lama = get_int("End size: ");
    }
    while (end_lama < start_lama);

    int start_lamas = start_lama;
    int years = 0;
    int desired_end = end_lama;

    while (desired_end > start_lamas)
    {
        int deaths = start_lamas / 4;
        int births = start_lamas / 3;
        int end_of_year = start_lamas - deaths + births;
        years++;
        start_lamas = end_of_year;
    }

    printf("Years: %i\n", years);
}
