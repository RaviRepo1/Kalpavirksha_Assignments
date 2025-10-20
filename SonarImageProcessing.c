#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE_INPUT 10
#define MIN_INTENSITY_VALUE 0
#define MAX_INTENSITY_VALUE 255

void printMatrix(const int *ptr, int n);
void inputMatrix(int *ptr, int n);
void generateRandomMatrix(int *ptr, int n, int minValue, int maxValue);
void swap(int *a, int *b);
void rotateMatrix90Clockwise(int *ptr, int n);
void applySmoothingFilter(int *ptr, int n);
int calculateNeighborsAvg(const int *ptr, const int *prevRow, int n, int row, int col);

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void printMatrix(const int *ptr, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%4d", *(ptr + i * n + j));
        }
        printf("\n");
    }
}

void inputMatrix(int *ptr, int n)
{
    printf("Enter %d elements row by row:\n", n * n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int result = scanf("%d", ptr + i * n + j);
            if (result != 1)
            {
                printf("Invalid input detected.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void generateRandomMatrix(int *ptr, int n, int minValue, int maxValue)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            *(ptr + i * n + j) = minValue + rand() % (maxValue - minValue + 1);
        }
    }
}

void rotateMatrix90Clockwise(int *ptr, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            swap(ptr + i * n + j, ptr + j * n + i);
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n / 2; j++)
        {
            swap(ptr + i * n + j, ptr + i * n + (n - 1 - j));
        }
    }
}

int calculateNeighborsAvg(const int *ptr, const int *prevRow, int n, int row, int col)
{
    int sum = 0, count = 0;

    for (int dx = -1; dx <= 1; dx++)
    {
        int x = row + dx;
        if (x < 0 || x >= n)
            continue;

        for (int dy = -1; dy <= 1; dy++)
        {
            int y = col + dy;
            if (y < 0 || y >= n)
                continue;

            if (dx == -1 && row > 0)
                sum += prevRow[y];
            else
                sum += *(ptr + x * n + y);

            count++;
        }
    }

    return sum / count;
}

void applySmoothingFilter(int *ptr, int n)
{
    int *prevRow = (int *)malloc(n * sizeof(int));
    int *currRow = (int *)malloc(n * sizeof(int));

    if (!prevRow || !currRow)
    {
        printf("Memory allocation failed.\n");
        free(prevRow);
        free(currRow);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            currRow[j] = calculateNeighborsAvg(ptr, prevRow, n, i, j);
        }

        for (int j = 0; j < n; j++)
        {
            prevRow[j] = *(ptr + i * n + j);
            *(ptr + i * n + j) = currRow[j];
        }
    }

    free(prevRow);
    free(currRow);
}

int main(void)
{
    int n;

    printf("Enter matrix size (2-%d): ", MAX_SIZE_INPUT);
    int result = scanf("%d", &n);

    if (result != 1 || n < 2 || n > MAX_SIZE_INPUT)
    {
        printf("Invalid matrix size. Must be between 2 and %d.\n", MAX_SIZE_INPUT);
        return EXIT_FAILURE;
    }

    int *ptr = malloc(n * n * sizeof(int));
    if (!ptr)
    {
        printf("Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    srand((unsigned)time(NULL));

    //------To Generate random matrix with values 0-255 (uncomment to use) ------
    generateRandomMatrix(ptr, n, MIN_INTENSITY_VALUE, MAX_INTENSITY_VALUE);

    //----- For a user-input matrix (for testing purpose) ------
    // inputMatrix(ptr, n);

    printf("\nOriginal Matrix (Random 0-255):\n");
    printMatrix(ptr, n);

    rotateMatrix90Clockwise(ptr, n);
    printf("\nMatrix after 90 degree Clockwise Rotation:\n");
    printMatrix(ptr, n);

    applySmoothingFilter(ptr, n);
    printf("\nMatrix after Applying Smoothing Filter:\n");
    printMatrix(ptr, n);

    free(ptr);
    return EXIT_SUCCESS;
}
