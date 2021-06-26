#ifndef CUE_H
#define CUE_H

#include <SDL2/SDL.h>
#include "ball.h"
#include "sdl_handler.h"
#include "vec.h"

extern LTexture gCueTexture;

struct Cue{
    int x,y,height=gCueTexture.getHeight(),width=gCueTexture.getWidth(),inside=0;
    double angle=0;
    point leftBottom,leftTop,rightTop,rightBottom,origin,currentLb;
    Vec lb,lt,rt,rb,currentDirection;

    void init(int i1,int i2,point c);

    void rotatePoints(double ang);

    int isInside(int x1,int y1);

    void handleEvent(SDL_Event& e);

    void render(Ball ball[]);
};

#endif
