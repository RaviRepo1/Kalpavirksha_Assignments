#include <string.h>
#include "utils.h"

extern MyTeams team[TotalTeams];

const char *roleToString(PlayerRole r)
{
    switch (r)
    {
    case ROLE_BATSMAN:
        return "Batsman";
    case ROLE_BOWLER:
        return "Bowler";
    case ROLE_ALL_ROUNDER:
        return "All-rounder";
    default:
        return "Unknown";
    }
}

PlayerRole getRoleFromString(const char *s)
{
    if (s == NULL)
        return ROLE_UNKNOWN;
    if (strcmp(s, "Batsman") == 0)
        return ROLE_BATSMAN;
    if (strcmp(s, "Bowler") == 0)
        return ROLE_BOWLER;
    if (strcmp(s, "All-rounder") == 0 || strcmp(s, "All rounder") == 0)
        return ROLE_ALL_ROUNDER;
    return ROLE_UNKNOWN;
}

int searchTeamById(int id)
{
    int low = 0;
    int high = TotalTeams - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (team[mid].id == id)
        {
            return mid;
        }
        else if (team[mid].id > id)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return -1;
}