#include <stdlib.h>
#include "hashmap.h"

int hashFunction(int key, int capacity) {
    return abs(key) % capacity;
}

HashMap* createHashMap(int capacity) {
    if (capacity <= 0) {
        return NULL;
    }
    
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    if (!map) {
        return NULL;
    }
    
    map->capacity = capacity;
    map->size = 0;
    map->buckets = (HashMapEntry**)calloc(capacity, sizeof(HashMapEntry*));
    if (!map->buckets) {
        free(map);
        return NULL;
    }
    
    return map;
}

StatusCode putHashMap(HashMap* map, int key, Node* node) {
    if (!map || !node) {
        return ERROR_NULL_POINTER;
    }
    
    int index = hashFunction(key, map->capacity);
    HashMapEntry* entry = map->buckets[index];
    
    
    while (entry) {
        if (entry->key == key) {
            entry->node = node;
            return SUCCESS;
        }
        entry = entry->next;
    }
    
   
    HashMapEntry* newEntry = (HashMapEntry*)malloc(sizeof(HashMapEntry));
    if (!newEntry) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    newEntry->key = key;
    newEntry->node = node;
    newEntry->next = map->buckets[index];
    map->buckets[index] = newEntry;
    map->size++;
    
    return SUCCESS;
}

Node* getHashMap(HashMap* map, int key) {
    if (!map) {
        return NULL;
    }
    
    int index = hashFunction(key, map->capacity);
    HashMapEntry* entry = map->buckets[index];
    
    while (entry) {
        if (entry->key == key) {
            return entry->node;
        }
        entry = entry->next;
    }
    
    return NULL;
}

StatusCode removeHashMap(HashMap* map, int key) {
    if (!map) {
        return ERROR_NULL_POINTER;
    }
    
    int index = hashFunction(key, map->capacity);
    HashMapEntry* entry = map->buckets[index];
    HashMapEntry* prev = NULL;
    
    while (entry) {
        if (entry->key == key) {
            if (prev) {
                prev->next = entry->next;
            } else {
                map->buckets[index] = entry->next;
            }
            free(entry);
            map->size--;
            return SUCCESS;
        }
        prev = entry;
        entry = entry->next;
    }
    
    return ERROR_KEY_NOT_FOUND;
}

StatusCode freeHashMap(HashMap* map) {
    if (!map) {
        return ERROR_NULL_POINTER;
    }
    
    for (int i = 0; i < map->capacity; i++) {
        HashMapEntry* entry = map->buckets[i];
        while (entry) {
            HashMapEntry* temp = entry;
            entry = entry->next;
            free(temp);
        }
    }
    
    free(map->buckets);
    free(map);
    return SUCCESS;
}
