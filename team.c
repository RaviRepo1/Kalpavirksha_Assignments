#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Players_data.h"
#include "team.h"
#include "utils.h"

MyTeams team[TotalTeams];

void initializeData()
{
  
    for (int index = 0; index < TotalTeams; index++)
    {
        team[index].id = index + 1;
        team[index].name = teams[index];
        team[index].totalPlayers = 0;
        team[index].averageBattingStrikeRate = 0.0;
        team[index].playerHead = NULL;
    }

    
    for (int index = 0; index < TotalPlayers; index++)
    {
        Player playerData = players[index];

        MyPlayer *p = (MyPlayer *)malloc(sizeof(MyPlayer));
        if (!p)
        {
            printf("Memory allocation failed while initializing players\n");
            return;
        }

        p->id = playerData.id;
        p->name = strdup(playerData.name); // own a copy so we can free later
        p->team = playerData.team;
        p->role = getRoleFromString(playerData.role); 
        p->totalRuns = playerData.totalRuns;
        p->battingAverage = playerData.battingAverage;
        p->strikeRate = playerData.strikeRate;
        p->wickets = playerData.wickets;
        p->economyRate = playerData.economyRate;
        p->next = NULL;

        
        if (p->role == ROLE_BATSMAN)
        {
            p->performanceIndex = (p->battingAverage * p->strikeRate) / 100;
        }
        else if (p->role == ROLE_BOWLER)
        {
            p->performanceIndex = (p->wickets * 2) + (100 - p->economyRate);
        }
        else if (p->role == ROLE_ALL_ROUNDER)
        {
            p->performanceIndex = ((p->battingAverage * p->strikeRate) / 100) + (p->wickets * 2);
        }
        else
        {
            p->performanceIndex = 0.0;
        }

        for (int iterator = 0; iterator < TotalTeams; iterator++)
        {
            if (strcmp(team[iterator].name, p->team) == 0)
            {
                p->next = team[iterator].playerHead;
                team[iterator].playerHead = p;
                team[iterator].totalPlayers++;
                break;
            }
        }
    }
}

void updateTeamAverageStrikeRate(MyTeams *team)
{
    float totalStrikeRate = 0.0;
    int count = 0;

    MyPlayer *temp = team->playerHead;

    while (temp != NULL)
    {
        if (temp->role == ROLE_BATSMAN || temp->role == ROLE_ALL_ROUNDER)
        {
            totalStrikeRate += temp->strikeRate;
            count++;
        }
        temp = temp->next;
    }

    team->averageBattingStrikeRate = (count > 0) ? totalStrikeRate / count : 0.0;
}

void addPlayerToTeam()
{
    int teamId;
    printf("Enter Team ID To Add Player: ");
    scanf("%d", &teamId);

    if (teamId < 1 || teamId > TotalTeams)
    {
        printf("Invalid Team ID\n");
        return;
    }

    int index = searchTeamById(teamId);

    if (index == -1)
    {
        printf("Team not found\n");
        return;
    }

    MyTeams *selectedTeam = &team[index];

    MyPlayer *newPlayer = (MyPlayer *)malloc(sizeof(MyPlayer));
    if (!newPlayer)
    {
        printf("Memory allocation failed\n");
        return;
    }

    printf("Enter Player Details: \n");

    printf("Player ID: ");
    scanf("%d", &newPlayer->id);
    getchar();

    char words[200];
    printf("Name: ");
    fgets(words, sizeof(words), stdin);
    words[strcspn(words, "\n")] = '\0';
    newPlayer->name = strdup(words); 

    newPlayer->team = selectedTeam->name;

    int roleChoice;
    printf("Role (1: Batsman 2: Bowler 3: All-rounder): ");
    scanf("%d", &roleChoice);

    if (roleChoice == 1)
    {
        newPlayer->role = ROLE_BATSMAN;
    }
    else if (roleChoice == 2)
    {
        newPlayer->role = ROLE_BOWLER;
    }
    else if (roleChoice == 3)
    {
        newPlayer->role = ROLE_ALL_ROUNDER;
    }
    else
    {
        printf("Invalid role choice\n");
        free(newPlayer->name);
        free(newPlayer);
        return;
    }

    printf("Total Runs: ");
    scanf("%d", &newPlayer->totalRuns);

    printf("Batting Average: ");
    scanf("%f", &newPlayer->battingAverage);

    printf("Strike Rate: ");
    scanf("%f", &newPlayer->strikeRate);

    printf("Wickets: ");
    scanf("%d", &newPlayer->wickets);

    printf("Economy Rate: ");
    scanf("%f", &newPlayer->economyRate);

    if (newPlayer->role == ROLE_BATSMAN)
    {
        newPlayer->performanceIndex = (newPlayer->battingAverage * newPlayer->strikeRate) / 100;
    }
    else if (newPlayer->role == ROLE_BOWLER)
    {
        newPlayer->performanceIndex = (newPlayer->wickets * 2) + (100 - newPlayer->economyRate);
    }
    else 
    {
        newPlayer->performanceIndex = ((newPlayer->battingAverage * newPlayer->strikeRate) / 100) + (newPlayer->wickets * 2);
    }

    newPlayer->next = selectedTeam->playerHead;
    selectedTeam->playerHead = newPlayer;
    selectedTeam->totalPlayers++;

    printf("Player added successfully to Team %s!\n", selectedTeam->name);
}

void displayPlayersOfSpecificTeam()
{
    int teamId;
    printf("Enter Team ID: ");
    if (scanf("%d", &teamId) != 1)
    {
        printf("Invalid input\n");
        
        return;
    }

    if (teamId < 1 || teamId > TotalTeams)
    {
        printf("Invalid Team ID\n");
        return;
    }

    int index = searchTeamById(teamId);
    if (index == -1)
    {
        printf("Team not found\n");
        return;
    }

    MyTeams *selectedTeam = &team[index]; 
    MyPlayer *head = selectedTeam->playerHead;

    printf("Players of Team %s:\n", selectedTeam->name);
    printf("=======================================================================================================\n");
    printf("%-5s %-20s %-15s %-8s %-8s %-8s %-8s %-8s %-12s\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("=======================================================================================================\n");

    if (head == NULL)
    {
        printf("No players found for this team\n");
        return;
    }

  
    updateTeamAverageStrikeRate(selectedTeam);

    MyPlayer *current = head;
    int printed = 0;
    while (current != NULL)
    {
        printf("%-5d %-20s %-15s %-8d %-8.2f %-8.2f %-8d %-8.2f %-12.2f\n",
               current->id,
               current->name,
               roleToString(current->role),
               current->totalRuns,
               current->battingAverage,
               current->strikeRate,
               current->wickets,
               current->economyRate,
               current->performanceIndex);

        printed++;
        current = current->next;
        if (printed >= selectedTeam->totalPlayers)
            break; 
    }

    printf("Total Players: %d\n", selectedTeam->totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", selectedTeam->averageBattingStrikeRate);
}

void displayTeamsByAverageBattingStrikeRate()
{
    for (int index = 0; index < TotalTeams; index++)
    {
        updateTeamAverageStrikeRate(&team[index]);
    }

    MyTeams sortedTeam[TotalTeams];

    for (int index = 0; index < TotalTeams; index++)
    {
        sortedTeam[index] = team[index];
    }

    for (int index = 0; index < TotalTeams - 1; index++)
    {
        for (int iterator = index + 1; iterator < TotalTeams; iterator++)
        {
            if (sortedTeam[index].averageBattingStrikeRate < sortedTeam[iterator].averageBattingStrikeRate)
            {
                MyTeams temp = sortedTeam[index];
                sortedTeam[index] = sortedTeam[iterator];
                sortedTeam[iterator] = temp;
            }
        }
    }

    printf("Team Sorted By Average Batting Strike Rate\n");
    printf("===============================================================\n");
    printf("%-5s %-25s %-20s %-5s\n", "ID", "Name", "Avg Bat SR", "Total Players");
    printf("===============================================================\n");

    for (int index = 0; index < TotalTeams; index++)
    {
        printf("%-5d %-25s %-20.2f %-5d\n", sortedTeam[index].id, sortedTeam[index].name, sortedTeam[index].averageBattingStrikeRate, sortedTeam[index].totalPlayers);
    }

    printf("===============================================================\n");
}