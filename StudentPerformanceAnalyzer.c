#include <stdio.h>
#include <string.h>

#define MAX_NUM_STUDENTS 100
#define TOTAL_SUBJECT 3

typedef struct
{
    int roll;
    char name[50];
    float marks[TOTAL_SUBJECT];
    float total;
    float average;
    char grade;
} Student;

// Funtions declarations
float calculateTotalMarks(const float marks[]);
float calculateAverageMarks(float total);
char getStudentGrade(float average);
void showStarsForGrade(char grade);
void printRollNumbersRecursively(const Student students[], int n, int index);

int main(void)
{
    int n;
    Student students[MAX_NUM_STUDENTS];
    
    printf("First line  : Number of students (e.g., 2)\n");
    printf("Next lines  : Roll_Number Name Marks1 Marks2 Marks3 for each student (e.g., 1 Raj 91 92 90)\n");
    printf("\n");
    scanf("%d", &n);

    int i = 0;
    while (i < n)
    {
        scanf("%d %s %f %f %f",
              &students[i].roll, students[i].name,
              &students[i].marks[0], &students[i].marks[1], &students[i].marks[2]);

        students[i].total = calculateTotalMarks(students[i].marks);
        students[i].average = calculateAverageMarks(students[i].total);
        students[i].grade = getStudentGrade(students[i].average);

        i++;
    }

    // Outputs section
    printf("\n");
    printf("========= STUDENT PERFORMANCE REPORT =========\n\n");

    i = 0;
    while (i < n)
    {
        printf("Roll       : %d\n", students[i].roll);
        printf("Name       : %s\n", students[i].name);
        printf("Total      : %d\n", (int)students[i].total);
        printf("Average    : %.2f\n", students[i].average);
        printf("Grade      : %c\n", students[i].grade);

        if (students[i].grade != 'F')
        {
            printf("Performance: ");
            showStarsForGrade(students[i].grade);
            printf("\n");
        }

        printf("\n");
        i++;
    }

    printf("List of Roll Numbers (via recursion): ");
    printRollNumbersRecursively(students, n, 0);

    return 0;
}

// Function definations
float calculateTotalMarks(const float marks[])
{
    float total = 0.0f;
    int i = 0;
    while (i < TOTAL_SUBJECT)
    {
        total += marks[i];
        i++;
    }
    return total;
}

float calculateAverageMarks(float total)
{
    return total / TOTAL_SUBJECT;
}

char getStudentGrade(float average)
{
    if (average >= 85.0f)
        return 'A';
    else if (average >= 70.0f)
        return 'B';
    else if (average >= 50.0f)
        return 'C';
    else if (average >= 35.0f)
        return 'D';
    else
        return 'F';
}

void showStarsForGrade(char grade)
{
    int stars = 0;
    switch (grade)
    {
    case 'A':
        stars = 5;
        break;
    case 'B':
        stars = 4;
        break;
    case 'C':
        stars = 3;
        break;
    case 'D':
        stars = 2;
        break;
    default:
        stars = 0;
        break;
    }

    int i = 0;
    while (i < stars)
    {
        printf("*");
        i++;
    }
}

void printRollNumbersRecursively(const Student students[], int n, int index)
{
    if (index == n)
        return;

    printf("%d ", students[index].roll);
    printRollNumbersRecursively(students, n, index + 1);
}
