#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *next;
};


void append(struct node **head, int data) {
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    newNode->data = data;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        struct node *temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }
}


void insertAtBegin(struct node **head, int data) {
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    newNode->data = data;
    newNode->next = *head;
    *head = newNode;
}


void insertAtEnd(struct node **head, int data) {
   
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    newNode->data = data;
    newNode->next = NULL;

   
    if (*head == NULL) {
        *head = newNode;
        return;
    }

   
    struct node *temp = *head;
    while (temp->next != NULL)
        temp = temp->next;

    
    temp->next = newNode;
}


void insertAtPos(struct node **head, int data, int pos) {
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    newNode->data = data;

    if (pos == 1) {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    struct node *temp = *head;
    for (int i = 1; i < pos - 1 && temp != NULL; i++)
        temp = temp->next;

    if (temp == NULL) {
        printf("Invalid position\n");
        free(newNode);
        return;
    }

    newNode->next = temp->next;
    temp->next = newNode;
}


void deleteAtBegin(struct node **head) {
    if (*head == NULL) {
        printf("List is empty\n");
        return;
    }
    struct node *temp = *head;
    *head = (*head)->next;
    free(temp);
}


void deleteAtEnd(struct node **head) {
    if (*head == NULL) {
        printf("List is empty\n");
        return;
    }

    struct node *temp = *head, *prev = NULL;
    if (temp->next == NULL) {
        free(temp);
        *head = NULL;
        return;
    }

    while (temp->next != NULL) {
        prev = temp;
        temp = temp->next;
    }

    prev->next = NULL;
    free(temp);
}


void deleteAtPos(struct node **head, int pos) {
    if (*head == NULL) {
        printf("List is empty\n");
        return;
    }

    if (pos == 1) {
        struct node *temp = *head;
        *head = (*head)->next;
        free(temp);
        return;
    }

    struct node *temp = *head, *prev = NULL;
    for (int i = 1; temp != NULL && i < pos; i++) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Invalid position\n");
        return;
    }

    prev->next = temp->next;
    free(temp);
}


void deleteByValue(struct node **head, int value) {
    struct node *temp = *head, *prev = NULL;

    if (temp != NULL && temp->data == value) {
        *head = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->data != value) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Value not found\n");
        return;
    }

    prev->next = temp->next;
    free(temp);
}


int count(struct node *head) {
    int c = 0;
    while (head != NULL) {
        c++;
        head = head->next;
    }
    return c;
}

void reverse(struct node **head) {
    struct node *prev = NULL, *curr = *head, *next = NULL;

    while (curr != NULL) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    *head = prev;
}


void display(struct node *head) {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }
    while (head != NULL) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

int main() {
    struct node *head = NULL;

    append(&head, 10);
    append(&head, 20);
    append(&head, 30);
    printf("After appending: ");
    display(head);

    insertAtBegin(&head, 5);
    printf("After inserting at beginning: ");
    display(head);

    insertAtPos(&head, 15, 3);
    printf("After inserting 15 at position 3: ");
    display(head);

    deleteAtBegin(&head);
    printf("After deleting from beginning: ");
    display(head);

    deleteAtEnd(&head);
    printf("After deleting from end: ");
    display(head);

    deleteAtPos(&head, 2);
    printf("After deleting node at position 2: ");
    display(head);

    append(&head, 25);
    append(&head, 35);
    deleteByValue(&head, 25);
    printf("After deleting value 25: ");
    display(head);

    printf("Total nodes = %d\n", count(head));

    reverse(&head);
    printf("After reversing list: ");
    display(head);

    return 0;
}
