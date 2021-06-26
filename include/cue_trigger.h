#ifndef CUE_TRIGGER_H
#define CUE_TRIGGER_H

#include <SDL2/SDL.h>
#include "sdl_handler.h"

extern LTexture gCueTriggerTexture;

struct CueSpeedTrigger{
    int x,y,height=gCueTriggerTexture.getHeight(),width=gCueTriggerTexture.getWidth(),inside,lim,initX;

    CueSpeedTrigger(int i1,int i2);

    int isInside(int x1,int y1);

    void handleEvent(SDL_Event& e,int &magnitude,int pauseTimerFlag);

    void render();
};

#endif
