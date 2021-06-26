/* This module controls the pages of "Introduction" section of the game */

#include <SDL2/SDL.h>
#include <stdio.h>
#include "include/sdl_handler.h"
#include "include/button.h"
#include "include/consts.h"

extern SDL_Renderer* gRenderer;
extern LTexture gIntroductionBgTexture[3];
extern LTexture gButtonBackTexture,gButtonNextTexture;
extern int windowMode;

static Button backButton,nextButton;
static int currentPage;

void initAllIntroductionMenu(){
	currentPage=0;
	backButton.init(86,613,gButtonBackTexture,1);
	nextButton.init(1115,613,gButtonNextTexture,1);
}

void handleEventIntroductionMenu(SDL_Event& e){
	if(e.type== SDL_MOUSEBUTTONDOWN && backButton.isMouseInside()){
		if(!currentPage){
			windowMode=HELP_MENU_WINDOW;
		}
		else currentPage--;
	}
	else if(e.type== SDL_MOUSEBUTTONDOWN && nextButton.isMouseInside()){
		if(currentPage<2)currentPage++;
	}
}

void renderAllIntroductionMenu(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	gIntroductionBgTexture[currentPage].render(0,0);
	backButton.render(gButtonBackTexture,1);
	if(currentPage<2)nextButton.render(gButtonNextTexture,1);

	SDL_RenderPresent( gRenderer );
}
