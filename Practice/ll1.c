#include <stdio.h>
#include <stdlib.h>



typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;


Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}


Node* insertEnd(Node* head, int data) {
    Node* newNode = createNode(data);

    if (head == NULL)
        return newNode;

    Node* temp = head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newNode;
    newNode->prev = temp;

    return head;
}


int* convertToArray(Node* head, int* size) {
    *size = 0;

    Node* temp = head;
    while (temp != NULL) {
        (*size)++;
        temp = temp->next;
    }

    int* arr = (int*)malloc((*size) * sizeof(int));

    temp = head;
    int i = 0;
    while (temp != NULL) {
        arr[i++] = temp->data;
        temp = temp->next;
    }

    return arr;
}

int main() {
    Node* head = NULL;

    
    head = insertEnd(head, 10);
    head = insertEnd(head, 20);
    head = insertEnd(head, 30);
    head = insertEnd(head, 40);

 
    int size;
    int* arr = convertToArray(head, &size);

    
    printf("Array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }

  
    free(arr);

    return 0;
}
