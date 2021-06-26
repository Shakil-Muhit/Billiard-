#ifndef LEADERBOARD_REPLICATION_H
#define LEADERBOARD_REPLICATION_H

#include <SDL2/SDL.h>

void initLeaderboardReplication();

int isEligibleLeaderboardReplication(int ms);

void updateLeaderboardReplication(char name[],char durationText[],int duration);

void handleEventLeaderboardReplication(SDL_Event &e);

void renderLeaderboardReplication();

#endif
