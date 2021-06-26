/* This module controls the circular and rectangular buttons. 
 * Button struct is for ball buttons and Rect_button struct controls
 * rectangular buttons in login menu */

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "include/sdl_handler.h"
#include "include/button.h"
#include "include/maths.h"
#include "include/consts.h"
#include "include/window_handler.h"

extern int windowMode,prevWindowMode;
extern LTexture gCueHoverTexture;

extern Mix_Chunk *gCueHitsBallHover;

void Button::init(int i,int j,LTexture &tex,int tar){
	height=tex.getHeight();
	width=tex.getWidth();
	x=i;
	y=j;
	r=width/2;
	targetWindow=tar;
	hover=0;
	prevHover=0;
	frame=0;
	angle=0.0;
}

int Button::isMouseInside(){
	int x1,y1;
	SDL_GetMouseState(&x1,&y1);
	//if(x1<x||x1>x+width||y1<y||y1>y+height)return 0;
	if(distanceSquared(x,y,x1,y1)>r*r)return 0;
	return 1;
}

void Button::handleButtonEvent(SDL_Event &e){


	if(e.type== SDL_MOUSEBUTTONDOWN && isMouseInside()){
		//prev_window_mode=-1;
		windowMode=targetWindow;
	}
}

void Button::render(LTexture &tex,int hoverFlag){
	tex.render(x-r,y-r,NULL,angle);

	if(isMouseInside()){
		hover=1;
		angle += 5;
		if(angle>359.9)angle=0;
	}
	else {
		hover=0;
		angle=0;
	}

	if(hover && hoverFlag){
		gCueHoverTexture.render(x-gCueHoverTexture.getWidth()/2,y+r);
	}

	if(hover && !prevHover)Mix_PlayChannel( -1, gCueHitsBallHover, 0 );
	prevHover=hover;
}

void RectButton::init(int i,int j,LTexture tex[]){
	height=tex[0].getHeight();
	width=tex[0].getWidth();
	x=i;
	y=j;
	hover=0;
	prevHover=0;
}

int RectButton::isMouseInside(){
	int x1,y1;
	SDL_GetMouseState(&x1,&y1);
	if(x1<x||x1>x+width||y1<y||y1>y+height)return 0;
	return 1;
}

void RectButton::render(LTexture tex[],int hoverFlag){
	if(isMouseInside()){
		hover=1;
	}
	else {
		hover=0;
	}

	if(hover && hoverFlag){
		tex[1].render(x,y);
	}
	else tex[0].render(x,y);

	if(hover && !prevHover)Mix_PlayChannel( -1, gCueHitsBallHover, 0 );
	prevHover=hover;
}
