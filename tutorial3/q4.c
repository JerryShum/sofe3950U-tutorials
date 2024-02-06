#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *professor = calloc(256, sizeof(char));
    int *student_ids;
    int *grades;

    printf("Enter your name, professor: ");
    scanf("%s", professor);
    printf("Enter the number of students: ");
    int num_students;
    scanf("%d", &num_students);

    student_ids = malloc(sizeof(int) * num_students);
    grades = malloc(sizeof(int) * num_students);

    free(professor);
    free(student_ids);
    free(grades);

    return 0;
}