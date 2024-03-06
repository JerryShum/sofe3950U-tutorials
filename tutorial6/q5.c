#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/**
 * @brief Create a program that does the following.
-   	A master process which prompts a user for five numbers and writes the five numbers to a file called numbers.txt
-   	The master process forks and creates a child process and then waits for the child process to terminate
-   	The child process reads the five numbers from numbers.txt and creates five threads
-   	Each thread executes a function factorial, which takes the number as an argument and does the following:
-   	Calculates the factorial (e.g. 5! = 5x4x3x2x1 = 120).
-   	Adds the factorial calculated to a global variable total_sum using the += operator
-   	The total_sum must be incremented in a thread-safe manner using semaphores
-   	Prints the current factorial value and the calculated factorial
-   	The child process has a join on all threads and after all threads have completed writes the total_sum to a file called sum.txt and terminates
-   	After the child process has terminated the parent process reads the contents of sum.txt and prints the total sum.
-   	Reading and writing files between processes is one of the simplest methods of IPC.
 */

int total_sum=0;
sem_t sem;

void *factorial(void *param) {
    int number = *((int *)param);
    int result = 1;
    for(int i = 1; i <= number; i++) {
        result *= i;
    }
    sem_wait(&sem);
    total_sum += result;
    sem_post(&sem);
    printf("Factorial of %d: %d\n", number, result);
    return NULL;
}

int main() {
    int numbers[5];
    printf("Enter 5 numbers: ");
    for(int i = 0; i < 5; i++) {
        scanf("%d", &numbers[i]);
    }
    FILE *file = fopen("numbers.txt", "w");
    for(int i = 0; i < 5; i++) {
        fprintf(file, "%d\n", numbers[i]);
    }
    fclose(file);

    pid_t pid = fork();
    if(pid == 0) {
        file = fopen("numbers.txt", "w");
        for(int i = 0; i < 5; i++) {
            fprintf(file, "%d\n", numbers[i]);
        }
        fclose(file);

        sem_init(&sem, 0, 1);

        pthread_t tid[5];
        for(int i = 0; i < 5; i++) {
            pthread_create(&tid[i], NULL, factorial, &numbers[i]);
        }

        for(int i = 0; i < 5; i++) {
            pthread_join(tid[i], NULL);
        }

        file = fopen("sum.txt", "w");
        fprintf(file, "%d\n", total_sum);
        fclose(file);

    } else {
        wait(NULL);

        file = fopen("sum.txt", "r");
        fscanf(file, "%d", &total_sum);
        fclose(file);
        printf("Total sum: %d\n", total_sum);
    }

    sem_destroy(&sem);

    return 0;    
}