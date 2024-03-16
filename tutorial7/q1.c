#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

struct proc
{
    char name[256];
    int priority;
    pid_t pid;
    int runtime;
};

struct queue
{
    struct proc proc;
    struct queue *next;
};

struct queue *head = NULL;

void push(struct proc process)
{
    // add each process to the linked list (queue)
    struct queue *new_node = (struct queue *)malloc(sizeof(struct queue));
    new_node->proc = process;
    new_node->next = NULL;
    if (head == NULL)
    {
        head = new_node;
    }
    else
    {
        struct queue *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

void free_queue()
{
    // free the queue
    struct queue *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main()
{
    // read process.txt (always 10 lines)
    FILE *file = fopen("processes.txt", "r");
    if (file == NULL)
    {
        printf("Error: process.txt not found\n");
        return 1;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '\n' || line[0] == '\0')
            break;

        struct proc process;
        sscanf(line, "%[^,],%d,%d,%d", process.name, &process.priority, &process.pid, &process.runtime);
        push(process);
    }

    fclose(file);

    // print queue
    struct queue *temp = head;
    while (temp != NULL)
    {
        printf("Name: %s, Priority: %d, PID: %d, Runtime: %d\n", temp->proc.name, temp->proc.priority, temp->proc.pid, temp->proc.runtime);
        temp = temp->next;
    }

    free_queue();

    return 0;
}
