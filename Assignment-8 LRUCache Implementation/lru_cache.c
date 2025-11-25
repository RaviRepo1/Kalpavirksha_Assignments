#include <stdlib.h>
#include <string.h>
#include "lru_cache.h"

LRUCache *createCache(int capacity)
{
    if (capacity <= 0)
    {
        return NULL;
    }

    LRUCache *cache = (LRUCache *)malloc(sizeof(LRUCache));
    if (!cache)
    {
        return NULL;
    }

    cache->map = createHashMap(capacity * 2);
    if (!cache->map)
    {
        free(cache);
        return NULL;
    }

    cache->head = NULL;
    cache->tail = NULL;
    cache->capacity = capacity;
    cache->size = 0;
    return cache;
}

StatusCode get(LRUCache *cache, int key, char **result)
{
    if (!cache || !result)
    {
        return ERROR_NULL_POINTER;
    }

    Node *node = getHashMap(cache->map, key);
    if (node == NULL)
    {
        *result = NULL;
        return ERROR_KEY_NOT_FOUND;
    }

    moveToFront(cache, node);
    *result = node->data;
    return SUCCESS;
}

StatusCode put(LRUCache *cache, int key, char *value)
{
    if (!cache || !value)
    {
        return ERROR_NULL_POINTER;
    }

    Node *node = getHashMap(cache->map, key);

    if (node != NULL)
    {
       
        char *newData = (char *)malloc(strlen(value) + 1);
        if (!newData)
        {
            return ERROR_MEMORY_ALLOCATION; 
        }
        strcpy(newData, value);

       
        free(node->data);
        node->data = newData;
        moveToFront(cache, node);
        return SUCCESS;
    }
    else
    {
        
        if (cache->size >= cache->capacity)
        {
            evictLRU(cache);
        }

        Node *newNode = createNode(key, value);
        if (!newNode)
        {
            return ERROR_MEMORY_ALLOCATION;
        }

        addToFront(cache, newNode);
        StatusCode status = putHashMap(cache->map, key, newNode);
        if (status != SUCCESS)
        {
            freeNode(newNode);
            return status;
        }
        cache->size++;
        return SUCCESS;
    }
}

void moveToFront(LRUCache *cache, Node *node)
{
    if (cache->head == node)
    {
        return;
    }
    removeNode(cache, node);
    addToFront(cache, node);
}

void addToFront(LRUCache *cache, Node *node)
{
    node->next = cache->head;
    node->prev = NULL;

    if (cache->head != NULL)
    {
        cache->head->prev = node;
    }
    cache->head = node;

    if (cache->tail == NULL)
    {
        cache->tail = node;
    }
}

void removeNode(LRUCache *cache, Node *node)
{
    if (node->prev != NULL)
    {
        node->prev->next = node->next;
    }
    else
    {
        cache->head = node->next;
    }

    if (node->next != NULL)
    {
        node->next->prev = node->prev;
    }
    else
    {
        cache->tail = node->prev;
    }
}

void evictLRU(LRUCache *cache)
{
    if (cache->tail == NULL)
    {
        return;
    }

    Node *lru = cache->tail;
    removeHashMap(cache->map, lru->key);
    removeNode(cache, lru);
    freeNode(lru);
    cache->size--;
}

StatusCode freeCache(LRUCache *cache)
{
    if (!cache)
    {
        return ERROR_NULL_POINTER;
    }

    Node *current = cache->head;
    while (current)
    {
        Node *temp = current;
        current = current->next;
        freeNode(temp);
    }

    freeHashMap(cache->map);
    free(cache);
    return SUCCESS;
}
