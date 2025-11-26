#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;


Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}


int getLength(Node* head) {
    int length = 0;
    while (head != NULL) {
        length++;
        head = head->next;
    }
    return length;
}


Node* findIntersection(Node* head1, Node* head2) {
    int len1 = getLength(head1);
    int len2 = getLength(head2);

    int diff = abs(len1 - len2);

    
    if (len1 > len2) {
        while (diff--) head1 = head1->next;
    }
    else {
        while (diff--) head2 = head2->next;
    }

    while (head1 != NULL && head2 != NULL) {
        if (head1 == head2)
            return head1;  
        head1 = head1->next;
        head2 = head2->next;
    }

    return NULL;  
}

int main() {
  

    Node* common = createNode(7);
    common->next = createNode(8);
    common->next->next = createNode(9);

    Node* head1 = createNode(1);
    head1->next = createNode(2);
    head1->next->next = createNode(3);
    head1->next->next->next = common;  

    Node* head2 = createNode(4);
    head2->next = createNode(5);
    head2->next->next = common;  

    Node* intersection = findIntersection(head1, head2);

    if (intersection != NULL)
        printf("Intersection at node with data: %d\n", intersection->data);
    else
        printf("No intersection found.\n");

    return 0;
}
