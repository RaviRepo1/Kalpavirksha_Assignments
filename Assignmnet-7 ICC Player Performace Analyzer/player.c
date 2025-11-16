#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "utils.h"

extern MyTeams team[TotalTeams];

int partition(MyPlayer *players[], int low, int high)
{
    float pivot = players[high]->performanceIndex;
    int index = low - 1;

    for (int iterator = low; iterator < high; iterator++)
    {
        if (players[iterator]->performanceIndex > pivot)
        {
            index++;
            MyPlayer *temp = players[index];
            players[index] = players[iterator];
            players[iterator] = temp;
        }
    }

    MyPlayer *temp = players[index + 1];
    players[index + 1] = players[high];
    players[high] = temp;

    return index + 1;
}

void quickSort(MyPlayer *players[], int low, int high, int numberOfPlayers)
{
    if (low < high)
    {
        int pivotIndex = partition(players, low, high);
        int leftCount = pivotIndex - low + 1;

        if (leftCount == numberOfPlayers)
        {
            return;
        }
        else if (leftCount > numberOfPlayers)
        {
            quickSort(players, low, pivotIndex - 1, numberOfPlayers);
        }
        else
        {
            quickSort(players, pivotIndex + 1, high, numberOfPlayers - leftCount);
        }
    }
}

void displayTopKPlayersOfASpecificTeamByRole()
{
    int teamId, roleId, numberOfPlayers;
    printf("Enter Team ID: ");
    scanf("%d", &teamId);

    if (teamId < 1 || teamId > TotalTeams)
    {
        printf("Invalid Team ID\n");
        return;
    }

    printf("Enter Role (1: Batsman 2: Bowler 3: All-rounder): ");
    scanf("%d", &roleId);

    PlayerRole role;
    if (roleId == 1)
    {
        role = ROLE_BATSMAN;
    }
    else if (roleId == 2)
    {
        role = ROLE_BOWLER;
    }
    else if (roleId == 3)
    {
        role = ROLE_ALL_ROUNDER;
    }
    else
    {
        printf("Invalid rold Id entered\n");
        return;
    }

    printf("Enter number of players: ");
    scanf("%d", &numberOfPlayers);

    MyTeams selectedTeam = team[teamId - 1];
    MyPlayer *current = selectedTeam.playerHead;

    MyPlayer *rolePlayers[200];
    int count = 0;

    while (current != NULL)
    {
        if (current->role == role)
        {
            rolePlayers[count++] = current;
        }
        current = current->next;
    }

    if (count == 0)
    {
        printf("No players found of %s role in Team %s\n", roleToString(role), selectedTeam.name);
        return;
    }

    if (numberOfPlayers > count)
    {
        numberOfPlayers = count;
    }

    quickSort(rolePlayers, 0, count - 1, numberOfPlayers);

    printf("Top %d %s of Team %s\n", numberOfPlayers, roleToString(role), selectedTeam.name);

    printf("=======================================================================================================\n");
    printf("%-5s %-20s %-15s %-8s %-8s %-8s %-8s %-8s %-12s\n", "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("=======================================================================================================\n");

    for (int index = 0; index < count && index < numberOfPlayers; index++)
    {
        MyPlayer *player = rolePlayers[index];
        printf("%-5d %-20s %-15s %-8d %-8.2f %-8.2f %-8d %-8.2f %-12.2f\n", player->id, player->name, roleToString(player->role), player->totalRuns, player->battingAverage, player->strikeRate, player->wickets, player->economyRate, player->performanceIndex);
    }

    printf("=======================================================================================================\n");
}

void swapHeap(HeapNode *first, HeapNode *second)
{
    HeapNode temp = *first;
    *first = *second;
    *second = temp;
}

void heapify(HeapNode heap[], int size, int index)
{
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < size && heap[left].player->performanceIndex > heap[largest].player->performanceIndex)
    {
        largest = left;
    }

    if (right < size && heap[right].player->performanceIndex > heap[largest].player->performanceIndex)
    {
        largest = right;
    }

    if (largest != index)
    {
        swapHeap(&heap[index], &heap[largest]);
        heapify(heap, size, largest);
    }
}

void displayAllPlayersOfAllTeams()
{
    int roleId;
    printf("Enter Role (1: Batsman 2: Bowler 3: All-rounder): ");
    if (scanf("%d", &roleId) != 1)
    {
        printf("Invalid input\n");
        return;
    }

    PlayerRole role;
    if (roleId == 1)
        role = ROLE_BATSMAN;
    else if (roleId == 2)
        role = ROLE_BOWLER;
    else if (roleId == 3)
        role = ROLE_ALL_ROUNDER;
    else
    {
        printf("Invalid choice for role.\n");
        return;
    }

    
    MyPlayer *finalHead = NULL;
    MyPlayer *finalTail = NULL;

   
    for (int t = 0; t < TotalTeams; ++t)
    {
       
        int cnt = 0;
        MyPlayer *cur = team[t].playerHead;
        while (cur)
        {
            if (cur->role == role) cnt++;
            cur = cur->next;
        }
        if (cnt == 0) continue;

        HeapNode *arr = (HeapNode *)malloc(cnt * sizeof(HeapNode));
        if (!arr)
        {
            printf("Memory allocation failed\n");

            MyPlayer *p = finalHead;
            while (p)
            {
                MyPlayer *n = p->next;
                free(p->name);
                free(p);
                p = n;
            }
            return;
        }

        
        int idx = 0;
        cur = team[t].playerHead;
        while (cur)
        {
            if (cur->role == role)
            {
                MyPlayer *copy = (MyPlayer *)malloc(sizeof(MyPlayer));
                if (!copy)
                {
                    printf("Memory allocation failed\n");
                  
                    for (int j = 0; j < idx; ++j)
                    {
                        free(arr[j].player->name);
                        free(arr[j].player);
                    }
                    free(arr);
                    MyPlayer *p = finalHead;
                    while (p)
                    {
                        MyPlayer *n = p->next;
                        free(p->name);
                        free(p);
                        p = n;
                    }
                    return;
                }
                memcpy(copy, cur, sizeof(MyPlayer));
                copy->next = NULL;
                copy->name = strdup(cur->name);

                arr[idx].player = copy;
                arr[idx].teamIndex = t;
                idx++;
            }
            cur = cur->next;
        }

      
        for (int start = (idx / 2) - 1; start >= 0; --start)
            heapify(arr, idx, start);

      
        int heapSize = idx;
        while (heapSize > 0)
        {
        
            HeapNode top = arr[0];
            MyPlayer *p = top.player;
            p->next = NULL;

        
            if (!finalHead)
            {
                finalHead = finalTail = p;
            }
            else
            {
                finalTail->next = p;
                finalTail = p;
            }

            
            arr[0] = arr[heapSize - 1];
            heapSize--;
            if (heapSize > 0)
                heapify(arr, heapSize, 0);
        }

        
        free(arr);
    }

   
    if (!finalHead)
    {
        printf("No players found for the selected role across all teams\n");
        return;
    }

    
    printf("%s of all teams (Each team heap-sorted desc, appended by team)\n", roleToString(role));
    printf("=======================================================================================================\n");
    printf("%-5s %-25s %-20s %-15s %-8s %-8s %-8s %-8s %-8s %-12s\n",
           "ID", "Name", "Team", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("=======================================================================================================\n");

    MyPlayer *iter = finalHead;
    while (iter)
    {
        printf("%-5d %-25s %-20s %-15s %-8d %-8.2f %-8.2f %-8d %-8.2f %-12.2f\n",
               iter->id,
               iter->name,
               iter->team ? iter->team : "Unknown",
               roleToString(iter->role),
               iter->totalRuns,
               iter->battingAverage,
               iter->strikeRate,
               iter->wickets,
               iter->economyRate,
               iter->performanceIndex);
        iter = iter->next;
    }
    printf("=======================================================================================================\n");

   
    iter = finalHead;
    while (iter)
    {
        MyPlayer *n = iter->next;
        if (iter->name) free(iter->name);
        free(iter);
        iter = n;
    }
}