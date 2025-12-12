#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "hashmap.h"

static int hashFunction(int pid) {
    return pid % HASHMAP_SIZE;
}

HashMap* createHashmap() {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    if (!map) {
        printf("Memory allocation failed for HashMap\n");
        exit(1);
    }
    
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        map->buckets[i] = NULL;
    }
    return map;
}

void hashmapInsert(HashMap* map, int pid, PCB* process) {
    int index = hashFunction(pid);
    
    HashNode* node = (HashNode*)malloc(sizeof(HashNode));
    if (!node) {
        printf("Memory allocation failed for HashNode\n");
        exit(1);
    }
    
    node->pid = pid;
    node->process = process;
    node->next = map->buckets[index];
    map->buckets[index] = node;
}

PCB* hashmapGet(HashMap* map, int pid) {
    int index = hashFunction(pid);
    HashNode* node = map->buckets[index];
    
    while (node != NULL) {
        if (node->pid == pid) {
            return node->process;
        }
        node = node->next;
    }
    return NULL;
}

void freeHashmap(HashMap* map) {
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        HashNode* node = map->buckets[i];
        while (node != NULL) {
            HashNode* temp = node;
            node = node->next;
            free(temp);
        }
    }
    free(map);
}