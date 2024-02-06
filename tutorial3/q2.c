
#include <stdio.h>

int main()
{
    FILE *file = fopen("question2.txt", "r");
    if(file == NULL)
    {
        printf("question2.txt could not be read.\n");
        return 1;
    }

    int numbers[10];
    for(int i = 0; i < 10; i++)
    {
        fscanf(file, "%d", &numbers[i]);
    }

    for(int i = 0; i < 10; i++)
    {
        printf("%d\n", numbers[i]);
    }
    fclose(file);
    return 0;
}