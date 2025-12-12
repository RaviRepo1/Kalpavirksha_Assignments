#ifndef PROCESS_H
#define PROCESS_H

#include "types.h"

PCB* createPcb(const char* name, int pid, int burstTime, int ioStart, int ioDuration);
void freePcb(PCB* pcb);

#endif