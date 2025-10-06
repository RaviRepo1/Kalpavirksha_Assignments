#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "stack.c"
#include <ctype.h>

int getPriority(char operator)
{
    return (operator == '+' || operator == '-') ? 1 : 2;
}

bool isOperand(char character)
{
    return character >= '0' && character <= '9';
}

bool isOperator(char character)
{
    return character == '+' || character == '-' || character == '*' || character == '/';
}


char* convertToPostfix(char infix[]) {
    static char postfix[2 * MAX_SIZE];
    Stack ops = {.top = -1, .capacity = MAX_SIZE};
    int i = 0, j = 0;

    while (infix[i]) {
        if (isdigit(infix[i])) {

            while (isdigit(infix[i])) postfix[j++] = infix[i++];
            postfix[j++] = ' ';
        }
        
         else if (infix[i] == ' ') {
            i++;
        } 
        
        else if (isOperator(infix[i])) {
           
            while (!isEmpty(&ops) && getPriority(peek(&ops)) >= getPriority(infix[i])) {
                postfix[j++] = pop(&ops);
                postfix[j++] = ' ';
            }
            push(&ops, infix[i++]);
        } 
        
        else {
            i++; 
        }
    }

    while (!isEmpty(&ops)) {
        postfix[j++] = pop(&ops);
        postfix[j++] = ' ';
    }

    postfix[j] = '\0';
    return postfix;
}


long evaluatePostfix(char infix[])
{
    char *postfix = convertToPostfix(infix);
    Stack valueStack = {.top = -1, .capacity = MAX_SIZE};

    int length = strlen(postfix);
    int number = 0;
    bool readingNumber = false;

    for (int i = 0; i < length; i++)
    {
        if (isOperand(postfix[i]))
        {
            number = number * 10 + (postfix[i] - '0');
            readingNumber = true;
        }
        else if (postfix[i] == ' ')
        {
            if (readingNumber)
            {
                push(&valueStack, number);
                number = 0;
                readingNumber = false;
            }
        }
        else
        {
            long operand2 = peek(&valueStack);
            pop(&valueStack);
            long operand1 = peek(&valueStack);
            pop(&valueStack);

            switch (postfix[i])
            {
            case '+':
                push(&valueStack, operand1 + operand2);
                break;
            case '-':
                push(&valueStack, operand1 - operand2);
                break;
            case '*':
                push(&valueStack, operand1 * operand2);
                break;
            case '/':
                if (operand2 == 0)
                {
                    printf("Error: Division by zero.\n");
                    return INT_MAX;
                }
                push(&valueStack, operand1 / operand2);
                break;
            }
        }
    }

    return peek(&valueStack);
}

int main()
{
    char inputExpression[100];
    char clearExpression[100];

    printf("Enter the Expression: ");
    scanf("%[^\n]s", inputExpression);

    int inputLength = strlen(inputExpression);
    int cleanIndex = 0;

    for (int i = 0; i < inputLength; i++)
    {
        char currentChar = inputExpression[i];

        if (!isOperand(currentChar) && !isOperator(currentChar) && currentChar != ' ')
        {
            printf("Error: Invalid character in expression.\n");
            return 1;
        }

        if (isOperator(currentChar))
        {
            if ((i + 1 < inputLength && isOperator(inputExpression[i + 1])) ||
                (i + 2 < inputLength && inputExpression[i + 1] == ' ' && isOperator(inputExpression[i + 2])))
            {
                printf("Error: Invalid expression.\n");
                return 1;
            }
        }

        clearExpression[cleanIndex++] = currentChar;
    }

    clearExpression[cleanIndex] = '\0';

    long result = evaluatePostfix(clearExpression);
    if (result != INT_MAX)
    {
        printf("Result: %ld\n", result);
    }

    return 0;
}
