#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *bellcurve(void *grade) {
    double *dgrade = (double *) grade;
    printf("Grade: %.2f, Bell Curved Grade: %.2f\n", *dgrade, (*dgrade)*1.50);
    return NULL;
}


int main() {
    pthread_t tids[5];
    char buffer[50];
    double grades[5];

    for(int i = 0; i < 5; i++) {
        printf("Enter grade: ");
        memset(buffer, 0, sizeof(char) * 50);
        fflush(stdin);
        fgets(buffer, 50, stdin);
        char *nl = strchr(buffer, '\n');
        if(nl) *nl = '\0';
        grades[i] = atof(buffer);
    }

    for(int i = 0; i < 5; i++) {
        pthread_create(&tids[i], NULL, bellcurve, (void *) &grades[i]);
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(tids[i], NULL);
    }

    return 0;
}