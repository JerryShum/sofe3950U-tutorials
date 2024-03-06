#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t total_bellcurve_lock;
pthread_mutex_t total_grade_lock;

double total_bellcurve = 0;
double total_grade = 0;

void *read_grades(void *param) {
    FILE *file = fopen("grades.txt", "r");
    for(int i = 0; i < 10; i++) {
        fscanf(file, "%d", (int *)param + i);
    }
    fclose(file);

    return NULL;
}

void *save_bellcurve(void *param) {
    int *grade = (int *)param;
    double dgrade = (double) *grade;
    pthread_mutex_lock(&total_bellcurve_lock);
    total_grade += dgrade;
    pthread_mutex_unlock(&total_bellcurve_lock);

    pthread_mutex_lock(&total_grade_lock);
    total_bellcurve += dgrade * 1.50;
    pthread_mutex_unlock(&total_grade_lock);
    
    return NULL;
}

void write_bellcurve() {
    FILE *file = fopen("bellcurve.txt", "w");
    fprintf(file, "%.2f\n", total_bellcurve/10);
    fclose(file);
}

int main() {
    pthread_mutex_init(&total_bellcurve_lock, NULL);
    pthread_mutex_init(&total_grade_lock, NULL);

    int grades[10];
    memset(grades, 0, sizeof(int) * 10);

    pthread_t tids[11];
    pthread_create(&tids[0], NULL, read_grades, (void *)grades);

    // simulates barrier
    pthread_join(tids[0], NULL);

    for(int i = 0; i < 10; i++) {
        printf("%d ", grades[i]);
    }
    printf("\n");

    for(int i = 0; i < 10; i++) {
        pthread_create(&tids[i + 1], NULL,  save_bellcurve, (void *)(grades));
    }
    // simulates barrier
    for(int i = 0; i < 10; i++) {
        pthread_join(tids[i + 1], NULL);
    }

    printf("Average: %.2f\n", total_grade / 10);
    printf("Bellcurve Average: %.2f\n", total_bellcurve / 10);
    write_bellcurve();

    return 0;
}