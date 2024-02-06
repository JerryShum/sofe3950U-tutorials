#include <stdio.h>

typedef struct student
{
    int student_id;
    int age;
    int year_started;
} student;

void save_student(const student student)
{
    FILE *file = fopen("students.txt", "a+");
    if(file == NULL)
    {
        printf("students.txt could not be written to.\n");
        return;
    }

    fprintf(file, "%d,%d,%d\n", student.student_id, student.age, student.year_started);
    fclose(file);
}

int main()
{
    student student;
    printf("Enter student id: ");
    scanf("%d", &student.student_id);
    printf("Enter student age: ");
    scanf("%d", &student.age);
    printf("Enter year started: ");
    scanf("%d", &student.year_started);
    save_student(student);
    return 0;
}