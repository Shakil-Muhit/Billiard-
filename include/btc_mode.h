#ifndef MODE_BTC_H
#define MODE_BTC_H

#include <SDL2/SDL.h>

static void applyPenalty(int points);

void initAllBtc();

void handleEventBtc(SDL_Event& e);

void renderAllBtc();

void ballScoreLogicBtc(int id);

void shotLogicBtc();

void gameWinLogicBtc();

void newShotLogicBtc();

int isValidAimBtc(int ballId);

#endif
