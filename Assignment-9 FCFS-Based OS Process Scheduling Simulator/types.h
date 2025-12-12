#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#define MAX_NAME_LEN 50
#define HASHMAP_SIZE 100
#define MAX_PROCESSES 100
#define MAX_KILL_EVENTS 100


typedef enum {
    READY,
    RUNNING,
    WAITING,
    TERMINATED
} ProcessState;

typedef struct {
    int pid;
    char name[MAX_NAME_LEN];
    int burstTime;
    int ioStartTime;
    int ioDuration;
    int executionTime;
    int ioRemaining;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    int arrivalTime;
    ProcessState state;
    bool isKilled;
    int killTime;
} PCB;


typedef struct QueueNode {
    PCB* process;
    struct QueueNode* next;
} QueueNode;


typedef struct {
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;


typedef struct HashNode {
    int pid;
    PCB* process;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode* buckets[HASHMAP_SIZE];
} HashMap;


typedef struct {
    int pid;
    int killTime;
} KillEvent;


typedef struct {
    HashMap* pcbMap;
    Queue* readyQueue;
    Queue* waitingQueue;
    Queue* terminatedQueue;
    PCB* runningProcess;
    int currentTime;
    PCB* allProcesses[MAX_PROCESSES];
    int processCount;
    KillEvent killEvents[MAX_KILL_EVENTS];
    int killEventCount;
} Scheduler;

#endif