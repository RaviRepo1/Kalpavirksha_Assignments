#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "types.h"

Scheduler* createScheduler();
void addProcess(Scheduler* sched, char* name, int pid, int burstTime, int ioStart, int ioDuration);
void addKillEvent(Scheduler* sched, int pid, int killTime);
void runScheduler(Scheduler* sched);
void printResults(Scheduler* sched);
void freeScheduler(Scheduler* sched);

#endif