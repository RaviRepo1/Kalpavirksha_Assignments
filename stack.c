#include "stack.h"
#include <stdio.h>

void push(Stack *s, long val) {
    if (s->top == s->capacity - 1) {
        printf("Stack overflow\n");
        return;
    }
    s->data[++s->top] = val;
}

long pop(Stack *s) {
    if (s->top == -1) {
        printf("Stack underflow\n");
        return -1;
    }
    return s->data[s->top--];
}

long peek(Stack *s) {
    if (s->top == -1) {
        printf("Stack empty\n");
        return -1;
    }
    return s->data[s->top];
}

bool isEmpty(Stack *s) {
    return s->top == -1;
}
