#ifndef UTILS_H
#define UTILS_H

#include "types.h"

const char *roleToString(PlayerRole r);
PlayerRole getRoleFromString(const char *s);
int searchTeamById(int id);

#endif