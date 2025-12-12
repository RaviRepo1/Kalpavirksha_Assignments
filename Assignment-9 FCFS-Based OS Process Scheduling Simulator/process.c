#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "process.h"

PCB* createPcb(const char* name, int pid, int burstTime, int ioStart, int ioDuration) {
    PCB* pcb = (PCB*)malloc(sizeof(PCB));
    if (!pcb) {
        printf("Memory allocation failed for PCB\n");
        exit(1);
    }
    
    strncpy(pcb->name, name, MAX_NAME_LEN - 1);
    pcb->name[MAX_NAME_LEN - 1] = '\0';
    pcb->pid = pid;
    pcb->burstTime = burstTime;
    pcb->ioStartTime = ioStart;
    pcb->ioDuration = ioDuration;
    pcb->executionTime = 0;
    pcb->ioRemaining = 0;
    pcb->completionTime = 0;
    pcb->turnaroundTime = 0;
    pcb->waitingTime = 0;
    pcb->arrivalTime = 0;
    pcb->state = READY;
    pcb->isKilled = false;
    pcb->killTime = -1;
    
    return pcb;
}

void freePcb(PCB* pcb) {
    if (pcb) {
        free(pcb);
    }
}