#ifndef TEAM_H
#define TEAM_H

#include "types.h"

void initializeData();
void updateTeamAverageStrikeRate(MyTeams *team);
void addPlayerToTeam();
void displayPlayersOfSpecificTeam();
void displayTeamsByAverageBattingStrikeRate();

#endif