#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
    char data;
    struct Node* prev;
    struct Node* next;
} Node;


Node* createNode(char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}


Node* insertEnd(Node* head, char data) {
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


char* convertToString(Node* head) {
    int length = 0;
    Node* temp = head;

  
    while (temp != NULL) {
        length++;
        temp = temp->next;
    }

  
    char* str = (char*)malloc((length + 1) * sizeof(char));


    temp = head;
    for (int i = 0; i < length; i++) {
        str[i] = temp->data;
        temp = temp->next;
    }

    str[length] = '\0';  
    return str;
}

int main() {
    Node* head = NULL;

   
    head = insertEnd(head, 'H');
    head = insertEnd(head, 'E');
    head = insertEnd(head, 'L');
    head = insertEnd(head, 'L');
    head = insertEnd(head, 'O');

    
    char* result = convertToString(head);

    printf("String: %s\n", result);

    free(result);
    return 0;
}
