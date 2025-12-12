#ifndef HASHMAP_H
#define HASHMAP_H

#include "types.h"

HashMap* createHashmap();
void hashmapInsert(HashMap* map, int pid, PCB* process);
PCB* hashmapGet(HashMap* map, int pid);
void freeHashmap(HashMap* map);

#endif