#include <stdlib.h>
#include <string.h>
#include "node.h"

Node* createNode(int key, char* data) {
    if (!data) {
        return NULL;
    }
    
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) {
        return NULL;
    }
    
    node->key = key;
    node->data = (char*)malloc(strlen(data) + 1);
    if (!node->data) {
        free(node);
        return NULL;
    }
    
    strcpy(node->data, data);
    node->prev = NULL;
    node->next = NULL;
    return node;
}

StatusCode freeNode(Node* node) {
    if (!node) {
        return ERROR_NULL_POINTER;
    }
    
    if (node->data) {
        free(node->data);
    }
    free(node);
    return SUCCESS;
}
