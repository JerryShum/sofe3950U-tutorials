#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

struct info
{
    int index;
    int number;
};

struct info infos[5] = {0};
int movingsum[5] = {0};
sem_t sems[5];

// Example: 4! with bound 2 = 4 * 3 * 1
int fac_recursive_bounded(int number, int bound)
{
    if (number == 0 || number == 1)
    {
        return 1;
    }
    if (number == bound)
    {
        printf("number: %d, bound: %d\n", number, bound);
        return 1;
    }
    return number * fac_recursive_bounded(number - 1, bound);
}

// 5! = 5 * 4 * 3!

void *fac(void *param)
{
    struct info info = *((struct info *)param);

    int i = info.index;
    int number = info.number;
    int bound = 0;
    sem_wait(&(sems[i]));
    printf("Started thread: %d\n", i);

    int result;
    if(i == 0) {
        result = fac_recursive_bounded(number, 1);
        movingsum[i] = result;
    } else {
        sem_wait(&(sems[i - 1]));
        bound = infos[i - 1].number;
        result  = fac_recursive_bounded(number, bound); // Example: 4! with bound 2 = 4 * 3 * 1
        movingsum[i] = result * movingsum[i - 1];
        sem_post(&(sems[i - 1]));
    }
    printf("Thread %d: %d! with bound %d = %d\n", i, number, bound, result);
    sem_post(&(sems[i]));
    return NULL;

}

int main()
{
    pthread_t tid[5];
    for(int i = 0; i < 5; i++){
        sem_init(&(sems[i]), 0, 1);
    }

    printf("enter 5 numbers in order (e.g. 1 2 4 5 10): ");
    for (int i = 0; i < 5; i++)
    {
        infos[i].index = i;
        scanf("%d", &infos[i].number);
    }

    printf("Starting with the threads: \n");
    for(int i = 0; i < 5; i++){
        printf("number: %d, index: %d\n", infos[i].number, infos[i].index);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_create(&tid[i], NULL, fac, &(infos[i]));
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_join(tid[i], NULL);
    }

    printf("Numbers: ");
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", infos[i].number);
    }
    printf("\nThe moving sum is: ");
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", movingsum[i]);
    }
    printf("\n");

    return 0;
}
