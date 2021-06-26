#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include <SDL2/SDL.h>
#include "sdl_handler.h"

struct Textbox{
	char inputText[50],defaultText[50];
	int textLen,renderText,onFocus,x,y,height,width,frame;

	int isMouseInside();

	void init(int x1,int y1,int w,int h,char def[]);

	void handleEvent(SDL_Event& e);

	void render(SDL_Color textColor, TTF_Font *font, int isPassword);
};

#endif
