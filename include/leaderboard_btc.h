#ifndef LEADERBOARD_BTC_H
#define LEADERBOARD_BTC_H

#include <SDL2/SDL.h>

void initLeaderboardBtc();

int isEligibleLeaderboardBtc(int ms);

void updateLeaderboardBtc(char name[],char durationText[],int duration);

void handleEventLeaderboardBtc(SDL_Event &e);

void renderLeaderboardBtc();

#endif
