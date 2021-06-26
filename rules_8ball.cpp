/* This modules controls the rendering of all pages of
 * the rules section of "8 Ball Pool" mode */

#include <SDL2/SDL.h>
#include <stdio.h>
#include "include/sdl_handler.h"
#include "include/button.h"
#include "include/consts.h"

extern SDL_Renderer* gRenderer;
extern LTexture gRules8ballBgTexture[3];
extern LTexture gButtonBackTexture,gButtonNextTexture;
extern int windowMode;

static Button backButton,nextButton;
static int currentPage;

void initAllRules8ball(){
	currentPage=0;
	backButton.init(86,613,gButtonBackTexture,1);
	nextButton.init(1115,613,gButtonNextTexture,1);
}

void handleEventRules8ball(SDL_Event& e){
	if(e.type== SDL_MOUSEBUTTONDOWN && backButton.isMouseInside()){
		if(!currentPage){
			windowMode=RULES_MENU_WINDOW;
		}
		else currentPage--;
	}
	else if(e.type== SDL_MOUSEBUTTONDOWN && nextButton.isMouseInside()){
		if(currentPage<2)currentPage++;
	}
}

void renderAllRules8ball(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	gRules8ballBgTexture[currentPage].render(0,0);
	backButton.render(gButtonBackTexture,1);
	if(currentPage<2)nextButton.render(gButtonNextTexture,1);

	SDL_RenderPresent( gRenderer );
}

