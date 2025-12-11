#ifndef STATUS_H
#define STATUS_H

typedef enum {
    SUCCESS = 0,
    ERROR_NULL_POINTER = 1,
    ERROR_KEY_NOT_FOUND = 2,
    ERROR_CACHE_FULL = 3,
    ERROR_INVALID_CAPACITY = 4,
    ERROR_MEMORY_ALLOCATION = 5,
    ERROR_DUPLICATE_KEY = 6
} StatusCode;

#endif
