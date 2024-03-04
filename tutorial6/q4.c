#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

/**
 * @brief The producer/consumer problem is a classic problem in synchronization, create a program that does the following.
-   	Create a global array buffer of length 5, this is shared by producer and consumers and initialized to zero.
-   	Prompts the user for ten numbers (store in an array use #define NUMBERS 10 for the size)
-   	Creates two threads, one a producer, the other a consumer
-   	The producer thread calls the function producer which takes the array of numbers from the users as an argument and does the following:
-   	Loops until all ten items have been added to the buffer, each time with a random delay before proceeding
-   	Using semaphores gets access to the critical section (buffer)
-   	For each number added to buffer prints “Produced <number>”, to indicate the number that has been added to the buffer
-   	If the buffer is full, it waits until a number has been consumed, so that another number can be added to the buffer
-   	The consumer thread calls the function consumer and does the following:
-   	Loops until ten items have been consumed from the buffer, each time with a random delay before proceeding
-   	Using semaphores gets access to the critical section (buffer)
-   	For each number consumed from the buffer, sets the buffer at that index to 0, indicating that the value has been consumed.
-   	For each number consumed, also prints “Consumed <number>”, to indicate the number that has been consumed from the buffer
-   	If the buffer is empty, it waits until a number has been added, so that another number can be consumed from the buffer
-   	The program waits for both threads to finish using join(), and then prints the contents of buffer, the contents of buffer should be all zeros.
 */

#define BUFFER_SIZE 5
#define NUMBERS_SIZE 10

int buffer[BUFFER_SIZE] = {0};
int buf_index = 0;

int numbers[NUMBERS_SIZE] = {0};
int num_index = 0;

int consumer_index = 0;
sem_t sem;

double random_delay() {
    // seed the random number generator
    srand(time(NULL));
    // random number between 1s to 2s
    return (double)rand() / (double)RAND_MAX + 1;
} 

void print_buffer() {
    printf("Buffer: [");
    for(int i = 0; i < BUFFER_SIZE; i++) {
        printf("%d ", buffer[i]);
    }
    printf("]\n");

}

void *producer(void *param) {
    while(num_index < NUMBERS_SIZE) {
        sem_wait(&sem);
        if(buf_index < BUFFER_SIZE) {
            buffer[buf_index] = numbers[num_index];
            printf("Produced %d\n", numbers[num_index]);
            buf_index++;
            num_index++;
            print_buffer();
        }
        sem_post(&sem);
        sleep(random_delay());
    }
    return NULL;
}

void *consumer(void *param) {
    while(consumer_index < NUMBERS_SIZE) {
        sem_wait(&sem);
        if(buf_index > 0) {
            printf("Consumed %d\n", buffer[buf_index - 1]);
            buffer[buf_index - 1] = 0;
            buf_index--;
            consumer_index++;
            print_buffer();
        }
        sem_post(&sem);
        sleep(random_delay()*3);
    }
}

int main() {

    pthread_t tids[2];

    sem_init(&sem, 0, 1);

    printf("Enter %d numbers (e.g. 1 2 3 4 5 6 7 8 9 10): ", NUMBERS_SIZE);
    for(int i = 0; i < NUMBERS_SIZE; i++) {
        scanf("%d", &numbers[i]);
    }

    printf("Numbers: ");
    for(int i = 0; i < NUMBERS_SIZE; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    pthread_create(&tids[0], NULL, producer, NULL);
    pthread_create(&tids[1], NULL, consumer, NULL);

    pthread_join(tids[0], NULL);
    pthread_join(tids[1], NULL);

    return 0;
}