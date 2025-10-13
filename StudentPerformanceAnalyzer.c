#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUM_STUDENTS 100
#define MAX_NAME_LENGTH 50
#define TOTAL_SUBJECTS 3

#define GRADE_A 85.0f
#define GRADE_B 70.0f
#define GRADE_C 50.0f
#define GRADE_D 35.0f
#define FAIL_GRADE 'F'

typedef struct
{
    int roll;
    char name[MAX_NAME_LENGTH];
    float marks[TOTAL_SUBJECTS];
    float total;
    float average;
    char grade;
} Student;

void readStudentData(Student students[], int n);
float calculateTotalMarks(const float marks[], int size);
float calculateAverageMarks(const float marks[], int size);
char getStudentGrade(float average);
void showStarsForGrade(char grade);
void printStudentReport(const Student *student);
void printRollNumbers(const Student students[], int n, int index);
void printAllStudentReports(const Student students[], int n);

int main(void)
{
    int n;
    Student students[MAX_NUM_STUDENTS];

    printf("First line  : Number of students (e.g., 2)\n");
    printf("Next lines  : Roll_Number Name Marks1 Marks2 Marks3 for each student (e.g., 1 Raj 91 92 90)\n\n");

    scanf("%d", &n);

    readStudentData(students, n);
    printAllStudentReports(students, n);

    printf("List of Roll Numbers (via recursion): ");
    printRollNumbers(students, n, 0);

    return 0;
}

void readStudentData(Student students[], int n)
{
    for (int i = 0; i < n; i++)
    {
        
        int validInput = 0;
        while (!validInput)
        {
            if (scanf("%d %s %f %f %f",
                      &students[i].roll, students[i].name,
                      &students[i].marks[0], &students[i].marks[1], &students[i].marks[2]) == 5)
            {
                
                if (students[i].roll <= 0)
                {
                    printf("Invalid roll number. Roll number must be positive. Try again:\n");
                    continue;
                }

                
                int validMarks = 1;
                for (int j = 0; j < TOTAL_SUBJECTS; j++)
                {
                    if (students[i].marks[j] < 0.0f || students[i].marks[j] > 100.0f)
                    {
                        printf("Invalid marks entered for subject %d. Marks must be between 0 and 100.\n", j + 1);
                        validMarks = 0;
                        break;
                    }
                }

                if (validMarks)
                    validInput = 1;
                else
                    printf("Please re-enter Roll, Name, and all 3 marks again:\n");
            }
            else
            {
                printf("Invalid input format. Please enter data as: Roll Name Marks1 Marks2 Marks3\n");
                while (getchar() != '\n')
                    ;
            }
        }
       
        students[i].total = calculateTotalMarks(students[i].marks, TOTAL_SUBJECTS);
        students[i].average = students[i].total / TOTAL_SUBJECTS; 
        students[i].grade = getStudentGrade(students[i].average);
    }
}

float calculateTotalMarks(const float marks[], int size)
{
    float total = 0.0f;
    for (int i = 0; i < size; i++)
    {
        total += marks[i];
    }
    return total;
}

char getStudentGrade(float average)
{
    if (average >= GRADE_A)
        return 'A';
    else if (average >= GRADE_B)
        return 'B';
    else if (average >= GRADE_C)
        return 'C';
    else if (average >= GRADE_D)
        return 'D';
    else
        return 'F';
}

void showStarsForGrade(char grade)
{
    switch (grade)
    {
    case 'A':
        printf("*****");
        break;
    case 'B':
        printf("****");
        break;
    case 'C':
        printf("***");
        break;
    case 'D':
        printf("**");
        break;
    default:
        break;
    }
}

void printStudentReport(const Student *student)
{
    printf("Roll       : %d\n", student->roll);
    printf("Name       : %s\n", student->name);
    printf("Total      : %d\n", (int)student->total);
    printf("Average    : %.2f\n", student->average);
    printf("Grade      : %c\n", student->grade);

    if (student->grade != FAIL_GRADE)
    {
        printf("Performance: ");
        showStarsForGrade(student->grade);
        printf("\n");
    }
}

void printRollNumbers(const Student students[], int n, int index)
{
    if (index == n)
        return;
    printf("%d ", students[index].roll);
    printRollNumbers(students, n, index + 1);
}

void printAllStudentReports(const Student students[], int n)
{
    printf("\n========= STUDENT PERFORMANCE REPORT =========\n\n");
    for (int i = 0; i < n; i++)
    {
        printStudentReport(&students[i]);
        printf("\n");
    }
}
