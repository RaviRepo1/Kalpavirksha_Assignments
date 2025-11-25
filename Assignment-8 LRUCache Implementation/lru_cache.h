#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include "hashmap.h"
#include "status.h"

typedef struct LRUCache {
    HashMap* map;
    Node* head;
    Node* tail;
    int capacity;
    int size;
} LRUCache;

LRUCache* createCache(int capacity);
StatusCode get(LRUCache* cache, int key, char** result);
StatusCode put(LRUCache* cache, int key, char* value);
StatusCode freeCache(LRUCache* cache);

void moveToFront(LRUCache* cache, Node* node);
void addToFront(LRUCache* cache, Node* node);
void removeNode(LRUCache* cache, Node* node);
void evictLRU(LRUCache* cache);


#endif
