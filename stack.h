#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#define MAX_SIZE 100

typedef struct {
    long data[MAX_SIZE];
    int top;
    int capacity;
} Stack;

void push(Stack *s, long val);
long pop(Stack *s);
long peek(Stack *s);
bool isEmpty(Stack *s);

#endif
