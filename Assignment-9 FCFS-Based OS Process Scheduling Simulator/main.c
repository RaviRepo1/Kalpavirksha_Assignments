#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "types.h"
#include "scheduler.h"
#include "queue.h"
#include "process.h"
#include "hashmap.h"

#include "scheduler.c"
#include "queue.c"
#include "process.c"
#include "hashmap.c"

void printUsage()
{
    printf("\n=== FCFS Process Scheduler ===\n");
    printf("Enter all process details and kill events, then type 'RUN' to start.\n");
    printf("Format:\n");
    printf("  Process: <process_name> <pid> <burst_time> <io_start_time> <io_duration>\n");
    printf("  Kill:    KILL <pid> <kill_time>\n");
    printf("\nExample:\n");
    printf("  chrome 101 10 4 3\n");
    printf("  vscode 102 8 3 2\n");
    printf("  KILL 102 7\n");
    printf("  RUN\n\n");
}

void trim(char *str)
{
    char *start = str;
    while (*start && isspace(*start))
    {
        start++;
    }

    char *end = start + strlen(start) - 1;
    while (end > start && isspace(*end))
    {
        end--;
    }
    *(end + 1) = '\0';

    if (start != str)
    {
        memmove(str, start, strlen(start) + 1);
    }
}

int main()
{
    Scheduler *scheduler = createScheduler();
    char input[256];
    printUsage();
    printf("Enter processes and kill events (type 'RUN' when done):\n");
    printf("-------------------------------------------------------\n");

    while (1)
    {
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\n")] = 0;
        trim(input);

        if (strlen(input) == 0)
        {
            continue;
        }

        if (strcmp(input, "RUN") == 0 || strcmp(input, "run") == 0)
        {
            break;
        }
        if (strcmp(input, "DONE") == 0 || strcmp(input, "done") == 0)
        {
            break;
        }

        if (strncmp(input, "KILL", 4) == 0 || strncmp(input, "kill", 4) == 0)
        {
            int pid, killTime;
            if (sscanf(input, "%*s %d %d", &pid, &killTime) == 2)
            {
                addKillEvent(scheduler, pid, killTime);
            }
            else
            {
                printf("Invalid KILL format. Use: KILL <pid> <kill_time>\n");
            }
        }

        else
        {
            char name[MAX_NAME_LEN];
            int pid, burstTime, ioStart, ioDuration;

            if (sscanf(input, "%s %d %d %d %d", name, &pid, &burstTime, &ioStart, &ioDuration) == 5)
            {
                addProcess(scheduler, name, pid, burstTime, ioStart, ioDuration);
            }
            else
            {
                printf("Invalid format. Use: <name> <pid> <burst> <io_start> <io_duration>\n");
            }
        }
    }
    printf("-------------------------------------------------------\n\n");
    if (scheduler->processCount == 0)
    {
        printf("No processes to schedule. Exiting.\n");
        freeScheduler(scheduler);
        return 0;
    }

    runScheduler(scheduler);
    printResults(scheduler);
    freeScheduler(scheduler);

    return 0;
}