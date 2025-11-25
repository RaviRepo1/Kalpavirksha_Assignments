#include <stdio.h>
#include <string.h>

#include "lru_cache.h"
#include "hashmap.h"
#include "node.h"

#include "lru_cache.c"
#include "hashmap.c"
#include "node.c"

#define MAX_COMMANDS 1000
#define MAX_VALUE_LEN 100

typedef struct
{
    char type[20];
    int key;
    char value[MAX_VALUE_LEN];
} Command;

int main()
{
    Command cmds[MAX_COMMANDS];
    int count = 0;

    char line[200];

    while (fgets(line, sizeof(line), stdin))
    {

        Command *c = &cmds[count];

        if (sscanf(line, "%s", c->type) != 1)
            continue;

        if (strcmp(c->type, "createCache") == 0)
        {
            sscanf(line, "%*s %d", &c->key);
        }
        else if (strcmp(c->type, "put") == 0)
        {
            sscanf(line, "%*s %d %s", &c->key, c->value);
        }
        else if (strcmp(c->type, "get") == 0)
        {
            sscanf(line, "%*s %d", &c->key);
        }
        else if (strcmp(c->type, "exit") == 0)
        {
            count++;
            break;
        }

        count++;
    }

    LRUCache *cache = NULL;
    int capacity = 0;

    for (int i = 0; i < count; i++)
    {

        Command *c = &cmds[i];

        if (strcmp(c->type, "createCache") == 0)
        {
            capacity = c->key;
            cache = createCache(capacity);

            if (!cache)
            {
                fprintf(stderr, "Error: Failed to create cache\n");
                return ERROR_MEMORY_ALLOCATION;
            }
        }

        else if (strcmp(c->type, "put") == 0)
        {
            StatusCode s = put(cache, c->key, c->value);

            if (s != SUCCESS)
            {
                fprintf(stderr, "Error: Failed to put key=%d\n", c->key);
            }
        }

        else if (strcmp(c->type, "get") == 0)
        {
            char *result = NULL;
            StatusCode s = get(cache, c->key, &result);

            if (s == SUCCESS && result)
                printf("%s\n", result);
            else
                printf("NULL\n");
        }

        else if (strcmp(c->type, "exit") == 0)
        {
            break;
        }
    }

    if (cache)
        freeCache(cache);

    return SUCCESS;
}
