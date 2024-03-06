#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t total_grade_lock;
double total_grade = 0;

void *class_total(void *param) {
    double grade = atof((char *)param);
    pthread_mutex_lock(&total_grade_lock);
    total_grade += grade;
    pthread_mutex_unlock(&total_grade_lock);
    free(param);
    return NULL;
}

int main() {
    pthread_mutex_init(&total_grade_lock, NULL);
    pthread_t tids[10];
    char buffer[50];
    for(int i = 0; i < 10; i++) {
        memset(buffer, 0, sizeof(char) * 50);
        fflush(stdin);
        printf("Enter grade for student %d: ", i+1);
        fgets(buffer, 50, stdin);
        char *nl = strchr(buffer, '\n');
        if(nl) *nl = '\0';

        char *copy = strdup(buffer);
        pthread_create(&tids[i], NULL, class_total, (void *) copy);
    }

    for(int i = 0; i < 10; i++) {
        pthread_join(tids[i], NULL);
    }
    printf("Total grade: %.2f\n", total_grade);
    return 0;
}
