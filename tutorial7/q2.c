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

struct proc *pop()
{
    struct proc *p = NULL;
    if (head == NULL)
    {
        printf("Stack is empty\n");
    }
    else
    {
        p = &(head->proc);
        struct queue *temp = head;
        head = head->next;
    }
    return p;
}

void delete_name(char *name)
{
    struct queue *temp = head;
    struct queue *prev = NULL;
    while (temp != NULL)
    {
        if (strcmp(temp->proc.name, name) == 0)
        {
            if (prev == NULL)
            {
                head = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

struct proc delete_pid(int pid)
{
    struct proc p;
    struct queue *temp = head;
    struct queue *prev = NULL;
    while (temp != NULL)
    {
        if (temp->proc.pid == pid)
        {
            p = temp->proc;
            if (prev == NULL)
            {
                head = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }
            return p;
        }
        prev = temp;
        temp = temp->next;
    }
    return p;
}

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

void populate_head()
{
    FILE *file = fopen("processes.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
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
}

void print_queue()
{
    struct queue *temp = head;
    while (temp != NULL)
    {
        printf("%s, %d, %d, %d\n", temp->proc.name, temp->proc.priority, temp->proc.pid, temp->proc.runtime);
        temp = temp->next;
    }
}

void free_queue()
{
    struct proc *proc;
    do
    {
        proc = pop();
        if (proc != NULL)
        {
            printf("Freeing %s, %d, %d, %d\n", proc->name, proc->priority, proc->pid, proc->runtime);
            free(proc);
        }
    } while (proc != NULL);
}

int main()
{
    populate_head();

    // should be default
    print_queue();
    printf("\n");

    // 1. delete process with named "emacs"
    printf("Deleting emacs...\n");
    delete_name("emacs");
    print_queue();
    printf("\n");

    // 2. delete process with pid 12235
    printf("Deleting pid 12235...\n");
    struct proc p = delete_pid(12235);
    printf("Deleted process: %s, %d, %d, %d\n", p.name, p.priority, p.pid, p.runtime);
    print_queue();
    printf("\n");

    free_queue();

    return 0;
}
