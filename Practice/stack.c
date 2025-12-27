#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};


void push(struct Node** top, int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    
    if (newNode == NULL) {
        printf("Stack Overflow: Memory allocation failed.\n");
        return;
    }

    newNode->data = value;
    newNode->next = *top; 
    *top = newNode;       

    printf("%d pushed to stack.\n", value);
}


int pop(struct Node** top) {
    if (*top == NULL) {
        printf("Stack Underflow: Stack is empty.\n");
        return -1;
    }

    struct Node* temp = *top;
    int poppedValue = temp->data;

    *top = (*top)->next;  
    free(temp);

    printf("%d popped from stack.\n", poppedValue);
    return poppedValue;
}


int peek(struct Node* top) {
    if (top == NULL) {
        printf("Stack is empty.\n");
        return -1;
    }
    return top->data;
}


int isEmpty(struct Node* top) {
    return (top == NULL);
}


void display(struct Node* top) {
    if (top == NULL) {
        printf("Stack is empty.\n");
        return;
    }

    struct Node* current = top;
    printf("Stack elements: ");
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main() {
    struct Node* top = NULL;   
    push(&top, 10);
    push(&top, 20);
    push(&top, 30);

    display(top);

    printf("Top element: %d\n", peek(top));

    pop(&top);
    display(top);

    printf("Is stack empty? %s\n", isEmpty(top) ? "Yes" : "No");

    pop(&top);
    pop(&top);

    printf("Is stack empty? %s\n", isEmpty(top) ? "Yes" : "No");

    pop(&top); // extra pop to test underflow

    return 0;
}
