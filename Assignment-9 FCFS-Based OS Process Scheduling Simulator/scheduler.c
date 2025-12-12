#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "scheduler.h"
#include "process.h"
#include "queue.h"
#include "hashmap.h"

Scheduler* createScheduler() {
    Scheduler* sched = (Scheduler*)malloc(sizeof(Scheduler));
    if (!sched) {
        printf("Memory allocation failed for Scheduler\n");
        exit(1);
    }
    
    sched->pcbMap = createHashmap();
    sched->readyQueue = createQueue();
    sched->waitingQueue = createQueue();
    sched->terminatedQueue = createQueue();
    sched->runningProcess = NULL;
    sched->currentTime = 0;
    sched->processCount = 0;
    sched->killEventCount = 0;
    
    return sched;
}

void addProcess(Scheduler* sched, char* name, int pid, int burstTime, int ioStart, int ioDuration) {
    PCB* pcb = createPcb(name, pid, burstTime, ioStart, ioDuration);
    hashmapInsert(sched->pcbMap, pid, pcb);
    enqueue(sched->readyQueue, pcb);
    sched->allProcesses[sched->processCount++] = pcb;
}

void addKillEvent(Scheduler* sched, int pid, int killTime) {
    sched->killEvents[sched->killEventCount].pid = pid;
    sched->killEvents[sched->killEventCount].killTime = killTime;
    sched->killEventCount++;
}

static void processKillEvents(Scheduler* sched) {
    for (int i = 0; i < sched->killEventCount; i++) {
        if (sched->killEvents[i].killTime == sched->currentTime) {
            int pid = sched->killEvents[i].pid;
            PCB* pcb = hashmapGet(sched->pcbMap, pid);
            
            if (pcb && !pcb->isKilled) {
                pcb->isKilled = true;
                pcb->killTime = sched->currentTime;
                pcb->completionTime = sched->currentTime;
                
                if (sched->runningProcess && sched->runningProcess->pid == pid) {
                    sched->runningProcess->state = TERMINATED;
                    enqueue(sched->terminatedQueue, sched->runningProcess);
                    sched->runningProcess = NULL;
                } else {
                    if (removeByPid(sched->readyQueue, pid)) {
                        pcb->state = TERMINATED;
                        enqueue(sched->terminatedQueue, pcb);
                    } else if (removeByPid(sched->waitingQueue, pid)) {
                        pcb->state = TERMINATED;
                        enqueue(sched->terminatedQueue, pcb);
                    }
                }    
                printf("Time %d: Process %d (%s) KILLED\n", sched->currentTime, pid, pcb->name);
            }
        }
    }
}

static void updateWaitingQueue(Scheduler* sched) {
    Queue* tempQueue = createQueue();
    
    while (!isEmpty(sched->waitingQueue)) {
        PCB* pcb = dequeue(sched->waitingQueue);
        
        if (pcb->ioRemaining > 0) {
            pcb->ioRemaining--;
            
            if (pcb->ioRemaining == 0) {
                pcb->state = READY;
                enqueue(sched->readyQueue, pcb);
                printf("Time %d: Process %d (%s) I/O completed, moved to Ready Queue\n", 
                       sched->currentTime, pcb->pid, pcb->name);
            } else {
                enqueue(tempQueue, pcb);
            }
        } else {
            enqueue(tempQueue, pcb);
        }
    }
    freeQueue(sched->waitingQueue);
    sched->waitingQueue = tempQueue;
}

void runScheduler(Scheduler* sched) {
    printf("\n========== FCFS Scheduler Started ==========\n\n");
    
    while (!isEmpty(sched->readyQueue) || sched->runningProcess != NULL || 
           !isEmpty(sched->waitingQueue)) {
        
        printf("--- Time %d ---\n", sched->currentTime);
        processKillEvents(sched);
        
        if (sched->runningProcess == NULL && !isEmpty(sched->readyQueue)) {
            sched->runningProcess = dequeue(sched->readyQueue);
            sched->runningProcess->state = RUNNING;
            printf("Time %d: Process %d (%s) started/resumed execution\n", 
                   sched->currentTime, sched->runningProcess->pid, sched->runningProcess->name);
        }
        
        if (sched->runningProcess != NULL) {
            PCB* current = sched->runningProcess;
            
            if (current->isKilled) {
                sched->runningProcess = NULL;
            } else {
                current->executionTime++;
                
                printf("Time %d: Process %d (%s) executing (progress: %d/%d)\n", 
                       sched->currentTime, current->pid, current->name, 
                       current->executionTime, current->burstTime);
                
                if (current->ioDuration > 0 && current->executionTime == current->ioStartTime) {
                    current->state = WAITING;
                    current->ioRemaining = current->ioDuration + 1;
                    enqueue(sched->waitingQueue, current);
                    printf("Time %d: Process %d (%s) requested I/O, moved to Waiting Queue\n", 
                           sched->currentTime + 1, current->pid, current->name);
                    sched->runningProcess = NULL;
                } else if (current->executionTime >= current->burstTime) {
                    current->state = TERMINATED;
                    current->completionTime = sched->currentTime + 1;
                    enqueue(sched->terminatedQueue, current);
                    printf("Time %d: Process %d (%s) completed execution\n", 
                           sched->currentTime + 1, current->pid, current->name);
                    sched->runningProcess = NULL;
                }
            }
        }
        
        sched->currentTime++;
        updateWaitingQueue(sched);
    }
    
    printf("\n========== All Processes Completed ==========\n\n");
}

void printResults(Scheduler* sched) {
    printf("\n========== Final Results ==========\n\n");
    printf("%-6s %-15s %-6s %-6s %-12s %-8s %-10s\n", 
           "PID", "Name", "CPU", "IO", "Turnaround", "Waiting", "Status");
    printf("-----------------------------------------------------------------------\n");
    
    for (int i = 0; i < sched->processCount; i++) {
        PCB* pcb = sched->allProcesses[i];
        
        if (pcb->isKilled) {
            printf("%-6d %-15s %-6d %-6d %-12s %-8s KILLED at %d\n", 
                   pcb->pid, pcb->name, pcb->burstTime, pcb->ioDuration,
                   "-", "-", pcb->killTime);
        } else {
            pcb->turnaroundTime = pcb->completionTime - pcb->arrivalTime;
            
            pcb->waitingTime = pcb->turnaroundTime - pcb->burstTime;
            
            printf("%-6d %-15s %-6d %-6d %-12d %-8d OK\n", 
                   pcb->pid, pcb->name, pcb->burstTime, pcb->ioDuration,
                   pcb->turnaroundTime, pcb->waitingTime);
        }
    }
    printf("\n");
}

void freeScheduler(Scheduler* sched) {
    for (int i = 0; i < sched->processCount; i++) {
        freePcb(sched->allProcesses[i]);
    }
    
    freeHashmap(sched->pcbMap);
    freeQueue(sched->readyQueue);
    freeQueue(sched->waitingQueue);
    freeQueue(sched->terminatedQueue);
    free(sched);
}