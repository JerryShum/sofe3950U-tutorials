#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct grade
{
    int student_id;
    int grade;
} grade;

void grade_students(grade *grades, int num_students)
{
    FILE *file = fopen("grades.txt", "w");
    if(file == NULL)
    {
        printf("grades.txt could not be written to.\n");
        return;
    }
    int grade_sum = 0;
    for(int i = 0; i < num_students; i++)
    {
        grade g = *(grades + i);
        grade_sum += g.grade;
        fprintf(file, "%d,%d\n", g.student_id, g.grade);
    }
    double average_grade = grade_sum / num_students;
    double standard_deviation = 0;
    for(int i = 0; i < num_students; i++)
    {
        grade g = *(grades + i);
        standard_deviation += pow(g.grade - average_grade, 2);
    }
    standard_deviation = sqrt(standard_deviation / num_students);
    fprintf(file, "Average grade: %f\n", average_grade);
    fprintf(file, "Standard deviation: %f\n", standard_deviation);
}

int main()
{
    char *professor = calloc(4096, sizeof(char));
    printf("Enter your name, professor: ");
    scanf("%s", professor);
    printf("Enter the number of students: ");
    int num_students;
    scanf("%d", &num_students);
    grade *grades = calloc(num_students, sizeof(grade));
    for(int i = 0; i < num_students; i++)
    {
        printf("Enter the student ID:");
        int student_id;
        scanf("%d", &student_id);
        printf("Enter the student's grade:");
        int student_grade;
        scanf("%d", &student_grade);
        grade g = {student_id, student_grade};
        *(grades + i) = g;
    }
    grade_students(grades, num_students);
    free(professor);
    free(grades);
    return 0;
}