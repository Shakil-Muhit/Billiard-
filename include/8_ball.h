#ifndef MODE_8_BALL_H
#define MODE_8_BALL_H

#include <SDL2/SDL.h>

void initAll8ball();

void handleEvent8ball(SDL_Event& e);

void renderAll8ball();

void ballScoreLogic8ball(int id);

void shotLogic8ball();

void gameWinLogic8ball();

void playerChangeLogic8ball();

void newShotLogic8ball();

int isValidAim8ball(int ballId);

#endif
