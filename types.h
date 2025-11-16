#ifndef TYPES_H
#define TYPES_H

#define TotalTeams 10
#define TotalPlayers 200

typedef enum
{
    ROLE_UNKNOWN = 0,
    ROLE_BATSMAN = 1,
    ROLE_BOWLER = 2,
    ROLE_ALL_ROUNDER = 3
} PlayerRole;

typedef struct MyPlayer
{
    int id;
    char *name;               
    const char *team;
    PlayerRole role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    struct MyPlayer *next;
} MyPlayer;

typedef struct
{
    int id;
    const char *name;
    int totalPlayers;
    float averageBattingStrikeRate;
    MyPlayer *playerHead;
} MyTeams;

typedef struct
{
    MyPlayer *player;
    int teamIndex;
} HeapNode;

#endif