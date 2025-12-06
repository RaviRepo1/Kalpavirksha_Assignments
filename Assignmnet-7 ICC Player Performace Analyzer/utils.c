#include <string.h>
#include "utils.h"

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
        return "Invalid Role";
    }
}

PlayerRole getRoleFromString(const char *s)
{
    if (s == NULL)
        return ROLE_BATSMAN; 
    if (strcmp(s, "Batsman") == 0)
        return ROLE_BATSMAN;
    if (strcmp(s, "Bowler") == 0)
        return ROLE_BOWLER;
    if (strcmp(s, "All-rounder") == 0 || strcmp(s, "All rounder") == 0)
        return ROLE_ALL_ROUNDER;
    return ROLE_BATSMAN; 
}

int searchTeamById(int id, MyTeams teams[])
{
    int low = 0;
    int high = TOTAL_TEAMS - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (teams[mid].id == id)
        {
            return mid;
        }
        else if (teams[mid].id > id)
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
