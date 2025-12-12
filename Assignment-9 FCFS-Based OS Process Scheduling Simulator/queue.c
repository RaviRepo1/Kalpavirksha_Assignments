#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "queue.h"

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (!q) {
        printf("Memory allocation failed for Queue\n");
        exit(1);
    }
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

void enqueue(Queue* q, PCB* process) {
    QueueNode* node = (QueueNode*)malloc(sizeof(QueueNode));
    if (!node) {
        printf("Memory allocation failed for QueueNode\n");
        exit(1);
    }
    node->process = process;
    node->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
    q->size++;
}

PCB* dequeue(Queue* q) {
    if (isEmpty(q)) {
        return NULL;
    }
    
    QueueNode* temp = q->front;
    PCB* process = temp->process;
    q->front = q->front->next;
    
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp);
    q->size--;
    return process;
}

int isEmpty(Queue* q) {
    return q->size == 0;
}

int removeByPid(Queue* q, int pid) {
    if (isEmpty(q)) {
        return 0;
    }
    
    QueueNode* curr = q->front;
    QueueNode* prev = NULL;
    
    while (curr != NULL) {
        if (curr->process->pid == pid) {
            if (prev == NULL) {
                q->front = curr->next;
                if (q->front == NULL) {
                    q->rear = NULL;
                }
            } else {
                prev->next = curr->next;
                if (curr == q->rear) {
                    q->rear = prev;
                }
            }
            free(curr);
            q->size--;
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return 0;
}

void freeQueue(Queue* q) {
    while (!isEmpty(q)) {
        dequeue(q);
    }
    free(q);
}