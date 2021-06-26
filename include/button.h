#ifndef BUTTON_H
#define BUTTON_H

#include "sdl_handler.h"

struct Button{
	int x,y,height,width,r,targetWindow,hover,frame,prevHover;
	double angle;

	void init(int i,int j,LTexture &tex,int tar);

	int isMouseInside();

	void handleButtonEvent(SDL_Event &e);

	void render(LTexture &tex,int hoverFlag);
};

struct RectButton{
	int x,y,height,width,hover,prevHover;

	void init(int i,int j,LTexture tex[]);

	int isMouseInside();

	void render(LTexture tex[],int hoverFlag);
};

#endif
