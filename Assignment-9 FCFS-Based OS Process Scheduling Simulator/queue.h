#ifndef QUEUE_H
#define QUEUE_H

#include "types.h"

Queue* createQueue();
void enqueue(Queue* q, PCB* process);
PCB* dequeue(Queue* q);
int isEmpty(Queue* q);
void freeQueue(Queue* q);
int removeByPid(Queue* q, int pid);

#endif