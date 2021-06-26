#ifndef MODE_9_BALL_H
#define MODE_9_BALL_H

#include <SDL2/SDL.h>

void initAll9ball();

void handleEvent9ball(SDL_Event& e);

void renderAll9ball();

void ballScoreLogic9ball(int id);

void shotLogic9ball();

void gameWinLogic9ball();

void playerChangeLogic9ball();

void newShotLogic9ball();

int isValidAim9ball(int ballId);

#endif
