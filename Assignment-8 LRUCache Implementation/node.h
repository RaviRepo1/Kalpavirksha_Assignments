#ifndef NODE_H
#define NODE_H

#include "status.h"

typedef struct Node {
    int key;
    char* data;
    struct Node* prev;
    struct Node* next;
} Node;

Node* createNode(int key, char* data);
StatusCode freeNode(Node* node);

#endif
