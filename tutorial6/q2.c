#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int main()
{
    pid_t pid = fork();
    if(pid == 0) {
        // child
        sleep(1);
        printf("Child process\n");
    } else {
        // parent
        int ret;
        wait(&ret);
        if(ret == 0) {
            // child executed successfully
            printf("Parent process\n");
        }
    }
    return 0;
}