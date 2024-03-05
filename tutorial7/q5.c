#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

struct proc
{
    char name[256];
    int priority;
    pid_t pid;
    int runtime;
};

struct ll_node
{
    struct proc process;
    struct ll_node *next;
};

struct ll_node *queue = NULL;

void add_node(struct ll_node *node)
{
    if (queue == NULL)
    {
        queue = node;
        return;
    }
    struct ll_node *curr = queue;
    while (curr->next != NULL)
    {
        curr = curr->next;
    }
    curr->next = node;
}

void populate_queue()
{
    printf("Populating queue\n");
    FILE *file = fopen("processes_q5.txt", "r");
    if (file == NULL)
    {
        printf("Bruh, can't open file\n");
        return;
    }

    char line[256];
    while (fgets(line, 256, file) && strlen(line) > 0)
    {
        struct ll_node *new_node = (struct ll_node *)malloc(sizeof(struct ll_node));
        memset(new_node, 0, sizeof(struct ll_node));

        struct proc *process = &(new_node->process);

        sscanf(line, "%[^,], %d, %d", process->name, &process->priority, &process->runtime);
        process->pid = 0; // default pid until it's actually executed

        add_node(new_node);
    }
}

void order_queue_by_priority_asc()
{
    struct ll_node *prev = NULL;
    struct ll_node *curr = queue;
    while (curr != NULL)
    {
        struct ll_node *next = curr->next;
        if (next != NULL && curr->process.priority > next->process.priority)
        {
            if (prev == NULL)
            {
                queue = next;
            }
            else
            {
                prev->next = next;
            }
            curr->next = next->next;
            next->next = curr;
        }
        prev = curr;
        curr = curr->next;
    }
}

struct proc *delete_name(char *name)
{
    struct ll_node *prev = NULL;
    struct ll_node *curr = queue;
    while (curr != NULL)
    {
        if (strcmp(curr->process.name, name) == 0)
        {
            if (prev == NULL)
            {
                queue = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            return &(curr->process);
        }
        prev = curr;
        curr = curr->next;
    }
    return NULL;
}

struct proc *pop()
{
    if (queue == NULL)
    {
        return NULL;
    }
    struct ll_node *node = queue;
    queue = queue->next;
    return &(node->process);
}

void free_queue()
{
    struct ll_node *curr = queue;
    while (curr != NULL)
    {
        struct ll_node *next = curr->next;
        free(curr);
        curr = next;
    }
}

void print_queue() {
    struct ll_node *curr = queue;
    while (curr != NULL)
    {
        printf("Name: %s, Priority: %d, Runtime: %d, PID: %d\n", curr->process.name, curr->process.priority, curr->process.runtime, curr->process.pid);
        curr = curr->next;
    }

}

int main()
{
    populate_queue();
    print_queue();

    pid_t pid;

    struct ll_node *q = queue;
    struct proc *p = NULL;

    // 1. do all 0 priorities first
    while (q != NULL)
    {
        p = &(q->process);
        if (p->priority == 0)
        {
            printf("Executing: %s, Priority: %d, Runtime: %d\n", p->name, p->priority, p->runtime);
            pid = fork();
            if (pid == 0)
            {
                sleep(p->runtime);
                execv(p->name, NULL);
            }
            else
            {
                p->pid = pid;
                waitpid(p->pid, NULL, 0);
                kill(p->pid, SIGINT);
                printf("Process | %s | with pid | %d | has finished\n", p->name, p->pid);
                q = q->next;
            }
        }

    }

    order_queue_by_priority_asc();

    // 2. do all other priorities
    p = NULL;
    do
    {
        p = pop();
        if (p != NULL)
        {
            printf("Executing: %s, Priority: %d, Runtime: %d\n", p->name, p->priority, p->runtime);
            pid = fork();
            if (pid == 0)
            {
                // child
                execv(p->name, NULL);
                sleep(p->runtime);
            }
            else
            {
                // parent
                p->pid = pid;
                waitpid(p->pid, NULL, 0);
                kill(p->pid, SIGINT);
                printf("Process | %s | with pid | %d | has finished\n", p->name, p->pid);
            }
        }
    } while (p != NULL);

    free_queue();

    return 0;
}