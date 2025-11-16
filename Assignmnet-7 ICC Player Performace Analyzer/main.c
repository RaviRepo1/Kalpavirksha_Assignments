#include <stdio.h>
#include <stdlib.h>
#include "team.h"
#include "player.h"
#include "team.c"
#include "player.c"
#include "utils.c"


extern MyTeams team[TotalTeams];

void exitPerformanceAnalyzer()
{
    for (int index = 0; index < TotalTeams; index++)
    {
        MyPlayer *temp = team[index].playerHead;

        while (temp != NULL)
        {
            MyPlayer *next = temp->next;
            if (temp->name)
                free(temp->name); 
            free(temp);
            temp = next;
        }
        team[index].playerHead = NULL;
    }

    printf("Exiting the program\n");
    exit(0);
}

int main()
{
    initializeData();

    while (1)
    {
        printf("=======================================================================================\n");
        printf("ICC ODI Player Performance Analyzer \n");
        printf("=======================================================================================\n");
        int choice;
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display All Players of Specific Role Across All Teams by Performance Index\n");
        printf("6. Exit\n");

        printf("=======================================================================================\n");

        if (scanf("%d", &choice) != 1)
        {
            
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            printf("Invalid choice\n");
            continue;
        }

        switch (choice)
        {
        case 1:
            addPlayerToTeam();
            break;
        case 2:
            displayPlayersOfSpecificTeam();
            break;
        case 3:
            displayTeamsByAverageBattingStrikeRate();
            break;
        case 4:
            displayTopKPlayersOfASpecificTeamByRole();
            break;
        case 5:
            displayAllPlayersOfAllTeams();
            break;
        case 6:
            exitPerformanceAnalyzer();
            break;
        default:
            printf("Invalid choice\n");
        }
    }

    return 0;
}