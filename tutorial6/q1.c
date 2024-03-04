#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int main()
{
    pid_t pid;
    pid = fork();
    if(pid == 0) {
        sleep(1);
        FILE *file = fopen("child1.txt", "w");
        fprintf(file, "child 1\n");
        fclose(file);
    } else {
        pid = fork();
        if(pid == 0) {
            sleep(1);
            FILE *file = fopen("child2.txt", "w");
            fprintf(file, "child 2\n");
            fclose(file);
        } else {
            FILE *file;

            // child 1
            file = fopen("child1.txt", "r");
            char line[100];
            fgets(line, 100, file);
            printf("%s", line);
            fclose(file);

            // child 2
            file = fopen("child2.txt", "r");
            fgets(line, 100, file);
            printf("%s", line);
            fclose(file);
        }
    }

}