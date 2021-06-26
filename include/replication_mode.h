#ifndef MODE_REPLICATION_H
#define MODE_REPLICATION_H

#include <SDL2/SDL.h>

static void generate_balls(int count);

void initAllReplication();

void handleEventReplication(SDL_Event& e);

void renderAllReplication();

void ballScoreLogicReplication(int id);

void shotLogicReplication();

void gameWinLogicReplication();

void newShotLogicReplication();

int isValidAimReplication(int ballId);

#endif
