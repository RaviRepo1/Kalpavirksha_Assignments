#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int is_operator(char c)
{
    return c == '+' || c == '-'||  c == '*' || c == '/';
}

int operator_precedence(char op)
{
    if (op == '*' || op == '/')
        return 2;
    if (op == '+' || op == '-')
        return 1;
    return 0;
}

float perform_operation(float a, float b, char op, int *error)
{
    switch (op)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if (b == 0)
        {
            *error = 1;
            return 0;
        }
        return a / b;
    default:
        *error = 1;
        return 0;
    }
}

void process_operator(float *numbers, int *top_numbers, char *operators, int *top_operators, int *error)
{
    if (*top_operators < 0 || *top_numbers < 1)
    {
        *error = 1;
        return;
    }
    float b = numbers[(*top_numbers)--];
    float a = numbers[(*top_numbers)--];
    char op = operators[(*top_operators)--];
    numbers[++(*top_numbers)] = perform_operation(a, b, op, error);
}

float evaluate_expression(const char *expression, int *error)
{
    float numbers[100];
    int top_numbers = -1;
    char operators[100];
    int top_operators = -1;

    int i = 0, n = strlen(expression);
    while (i < n && !*error)
    {
        if (isspace(expression[i]))
        {
            i++;
            continue;
        }

        if (isdigit(expression[i]) || expression[i] == '.')
        {
            float num = 0;
            int decimal_point = 0;
            float divisor = 1;

            while (i < n && (isdigit(expression[i]) || expression[i] == '.'))
            {
                if (expression[i] == '.')
                {
                    decimal_point = 1;
                }
                else
                {
                    num = num * 10 + (expression[i] - '0');
                    if (decimal_point)
                    {
                        divisor *= 10;
                    }
                }
                i++;
            }
            num /= divisor;
            numbers[++top_numbers] = num;
        }
        else if (is_operator(expression[i]))
        {
            while (top_operators >= 0 &&
                   operator_precedence(operators[top_operators]) >= operator_precedence(expression[i]))
            {
                process_operator(numbers, &top_numbers, operators, &top_operators, error);
            }
            operators[++top_operators] = expression[i];
            i++;
        }
        else
        {
            *error = 1;
        }
    }

    while (top_operators >= 0 && !*error)
    {
        process_operator(numbers, &top_numbers, operators, &top_operators, error);
    }

    if (top_numbers == 0 && !*error)
    {
        return numbers[top_numbers];
    }

    *error = 1;
    return 0;
}

int main()
{
    char input[100];
    printf("Enter the expression for calculation: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    int error = 0;
    float result = evaluate_expression(input, &error);

    if (error)
    {
        printf("Error: Invalid expression.\n");
    }
    else
    {
        printf("Result: %.2f\n", result);
    }

    return 0;
}