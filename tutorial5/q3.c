#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct student {
    char name[4096];
    char studentid[4096];
    double grade;
} student;

void *bellcurve(student *stu) {
    printf("Name: %s, Student ID: %s, Grade: %.2f, Bell Curved Grade: %.2f\n", stu->name, stu->studentid, stu->grade, (stu->grade)*1.50);
    return NULL;
}

int main() {
    pthread_t tids[5];
    char buffer[4096];
    student students[5];
    for(int i = 0; i < 5; i++) {
        printf("Enter name, studentid, and grade (e.g. jeremy,100849092,100.0): ");
        memset(buffer, 0, sizeof(char) * 4096);
        fflush(stdin);
        fgets(buffer, 4096, stdin);
        char *nl = strchr(buffer, '\n');
        if(nl) *nl = '\0';

        strcpy(students[i].name, strtok(buffer, ","));
        strcpy(students[i].studentid, strtok(NULL, ","));
        (&(students[i]))->grade = atof(strtok(NULL, ","));
    }

    for(int i = 0; i < 5; i++) {
        pthread_create(&tids[i], NULL, bellcurve, (void *) &(students[i]));
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(tids[i], NULL);
    }

}