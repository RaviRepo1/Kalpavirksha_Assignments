#ifndef HASHMAP_H
#define HASHMAP_H

#include "node.h"
#include "status.h"

typedef struct HashMapEntry {
    int key;
    Node* node;
    struct HashMapEntry* next;
} HashMapEntry;

typedef struct HashMap {
    HashMapEntry** buckets;
    int capacity;
    int size;
} HashMap;

HashMap* createHashMap(int capacity);
StatusCode putHashMap(HashMap* map, int key, Node* node);
Node* getHashMap(HashMap* map, int key);
StatusCode removeHashMap(HashMap* map, int key);
StatusCode freeHashMap(HashMap* map);

#endif
