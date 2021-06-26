#ifndef MODE_BTC_H
#define MODE_BTC_H

#include <SDL2/SDL.h>

struct ModifiedBall{
	int id,r,points;

	void init(int ind,int pt);

	void render(int x,int y);
};

void initModifiedBall();

void handleEventModifiedBall(SDL_Event &e);

void renderModifiedPipe();

#endif
