#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "ball.h"
#include "cue.h"

void checkMovement(Ball ball[],Cue &cue,int restartTimeFlag);

void declareWinner();

#endif
