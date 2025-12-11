#ifndef TEAM_H
#define TEAM_H

#include "types.h"

void initializeData();
MyTeams* getTeamByIndex(int index);
MyTeams* getTeamById(int teamId);
int getTotalTeams();

void updateTeamAverageStrikeRate(MyTeams *team);
void addPlayerToTeam();
void displayPlayersOfSpecificTeam();
void displayTeamsByAverageBattingStrikeRate();

#endif
