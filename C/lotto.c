#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_VAL 49

void reset(int* vals)
{
    for(int i = 0; i < MAX_VAL; i++)
        vals[i] = i + 1;
}

void shuffle(int* vals)
{
    int temp, r;
    for(int i = MAX_VAL; i > 0; i--)
    {
        r = rand() % i;
        temp = vals[i - 1];
        vals[i - 1] = vals[r];
        vals[r] = temp;
    }
}

int main(int argc, char** argv)
{
    int draws, length;

    srand(time(NULL));

    int vals[MAX_VAL];
    
    printf("%s\n", "How many draws?");
    scanf("%d", &draws);

    printf("%s\n", "How long?");
    scanf("%d", &length);

    for(int i = 0; i < draws; i++)
    {
        reset(vals);
        shuffle(vals);

        for(int j = 0; j < length; j++)
            printf("%d ", vals[j]);
        printf("%s", "\n");
    }

    return EXIT_SUCCESS;
}