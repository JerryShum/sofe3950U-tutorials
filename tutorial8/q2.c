#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define BUFFER_SIZE 256
#define MEMORY 1024

struct proc
{
    char name[BUFFER_SIZE];
    int priority;
    int pid;
    int address;
    int memory;
    int runtime;
    bool suspended;
};

struct proc_queue_node
{
    struct proc process;
    struct proc_queue_node *next;
};

struct proc_queue
{
    struct proc_queue_node *head;
    struct proc_queue_node *tail;
};

void push(struct proc_queue *queue, struct proc_queue_node *node)
{
    if (queue->head == NULL)
    {
        queue->head = node;
        queue->tail = node;
    }
    else
    {
        queue->tail->next = node;
        queue->tail = node;
    }
}

struct proc_queue_node *pop(struct proc_queue *queue)
{
    if (queue->head == NULL)
    {
        return NULL;
    }
    struct proc_queue_node *node = queue->head;
    queue->head = queue->head->next;
    return node;
}

void print_queue(struct proc_queue *queue)
{
    struct proc_queue_node *current = queue->head;
    while (current != NULL)
    {
        printf("Name: %s, Priority: %d, Memory: %d, Runtime: %d\n", current->process.name, current->process.priority, current->process.memory, current->process.runtime);
        current = current->next;
    }
}

void sort_by_priority_asc(struct proc_queue *queue)
{
    // sort the queue by priority in ascending order
    struct proc_queue_node *current = queue->head;
    while (current != NULL)
    {
        struct proc_queue_node *next = current->next;
        while (next != NULL)
        {
            if (current->process.priority > next->process.priority)
            {
                struct proc temp = current->process;
                current->process = next->process;
                next->process = temp;
            }
            next = next->next;
        }
        current = current->next;
    }
}

void populate_priority_and_secondary_queues(struct proc_queue *priority, struct proc_queue *secondary)
{
    FILE *file = fopen("processes_q2.txt", "r");
    if (file == NULL)
    {
        printf("Error: Could not open file\n");
        return;
    }

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, file))
    {
        struct proc_queue_node *node = (struct proc_queue_node *)malloc(sizeof(struct proc_queue_node));
        memset(node, 0, sizeof(struct proc_queue_node));

        struct proc *process = &node->process;
        char *name_str = strtok(buffer, ",");
        char *priority_str = strtok(NULL, ",");
        char *memory_str = strtok(NULL, ",");
        char *runtime_str = strtok(NULL, "");
        strcpy(process->name, name_str);
        process->priority = atoi(priority_str);
        process->memory = atoi(memory_str);
        process->runtime = atoi(runtime_str);

        process->suspended = false;
        process->pid = 0;
        process->address = -1;

        if (process->priority == 0)
        {
            push(priority, node);
        }
        else
        {
            push(secondary, node);
        }
    }
    sort_by_priority_asc(secondary);
}

// avail_mem = [1, 1, 1, 1, ..., 0, 0, 0]
// when memory = 256, take 256 from avail_mem and turn to 1's
int take_memory(char *avail_mem, int memory)
{
    int i = 0;
    int count = 0;
    while (count < memory)
    {
        if (avail_mem[i] == 0)
        {
            avail_mem[i] = 1;
            count++;
        }
        i++;
    }
    if (count < memory)
    {
        // no more available memory
        // turn back the 1's to 0's
        for(int j = i - 1; j >= 0; j--) {
            if(avail_mem[j] == 1) {
                avail_mem[j] = 0;
            }
        }
        return -1; // No more available memory
    }
    return i;
}

int give_memory_back(char *avail_mem, int address, int memory)
{
    // turn the memory back to 0's
    // address would be 256
    // memory would be something like 256
    // turn indices [0, 255] to 0's
    int start_index = address - memory;
    int end_index = address - 1;
    for (int i = start_index; i <= end_index; i++)
    {
        avail_mem[i] = 0;
    }
    return start_index;
}

void start_infinite_process(struct proc *process)
{
    int pid = fork();
    if (pid == 0)
    {
        // child
        while (1)
        {
            printf("Process %s is running for %d seconds with pid %d. Address block memory = %d\n", process->name, process->runtime, getpid(), process->address);
            sleep(1);
        }
        exit(0);
    }
    else
    {
        // parent
        process->pid = pid;
    }
}

void pause_process(struct proc *process)
{
    kill(process->pid, SIGTSTP);
    process->suspended = true;
}

void continue_process_for_1_second(struct proc *process)
{
    kill(process->pid, SIGCONT);
    process->suspended = false;
    sleep(1);
    kill(process->pid, SIGTSTP);
    process->suspended = true;
    // decrement runtime
    process->runtime--;
}

void continue_process_for_runtime(struct proc *process)
{
    kill(process->pid, SIGCONT);
    sleep(process->runtime);
    kill(process->pid, SIGINT);
}

void print_amount_of_available_and_non_available_memory(char *avail_mem)
{
    int available = 0;
    int non_available = 0;
    for (int i = 0; i < MEMORY; i++)
    {
        if (avail_mem[i] == 0)
        {
            available++;
        }
        else
        {
            non_available++;
        }
    }
    printf("Available: %d\n", available);
    printf("Non-available: %d\n", non_available);
}

int main()
{
    char *avail_mem = (char *)malloc(MEMORY * sizeof(char));
    memset(avail_mem, 0, MEMORY * sizeof(char));
    print_amount_of_available_and_non_available_memory(avail_mem);

    struct proc_queue priority;
    memset(&priority, 0, sizeof(struct proc_queue));

    struct proc_queue secondary;
    memset(&secondary, 0, sizeof(struct proc_queue));

    populate_priority_and_secondary_queues(&priority, &secondary);
    printf("Priority:\n");
    print_queue(&priority);
    printf("Secondary:\n");
    print_queue(&secondary);

    struct proc_queue_node *current;
    int addr;

    // iterate through priority queue first
    current = pop(&priority);
    while(current != NULL) {
        addr = take_memory(avail_mem, current->process.memory);
        if(addr == -1) {
            printf("No more available memory...\n");
            push(&secondary, current);
            sleep(3);
            break;
        }
        current->process.address = addr;
        printf("address: %d\n", addr);
        print_amount_of_available_and_non_available_memory(avail_mem);

        start_infinite_process(&current->process);
        continue_process_for_runtime(&current->process);
        waitpid(current->process.pid, NULL, 0);
        give_memory_back(avail_mem, current->process.address, current->process.memory);
        print_amount_of_available_and_non_available_memory(avail_mem);
        free(current);
        current = pop(&priority);
    }

    // iterate through secondary queue
    current = pop(&secondary);
    while(current != NULL) {
        addr = take_memory(avail_mem, current->process.memory);
        if(addr == -1) {
            printf("No more available memory...\n");
            sleep(5);
            break;
        }
        current->process.address = addr;
        printf("address: %d\n", addr);
        print_amount_of_available_and_non_available_memory(avail_mem);

        start_infinite_process(&current->process);
        pause_process(&current->process);
        continue_process_for_1_second(&current->process);
        // waitpid(current->process.pid, NULL, 0);
        give_memory_back(avail_mem, current->process.address, current->process.memory);
        print_amount_of_available_and_non_available_memory(avail_mem);
        if(current->process.runtime > 0) {
            push(&secondary, current);
        } else {
            kill(current->process.pid, SIGINT);
            free(current);
        }
        current = pop(&secondary);
    }

    return 0;
}