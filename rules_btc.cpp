/* This modules controls the rendering of all pages of
 * the rules section of "Beat the Clock" mode */

#include <SDL2/SDL.h>
#include <stdio.h>
#include "include/sdl_handler.h"
#include "include/button.h"
#include "include/consts.h"

extern SDL_Renderer* gRenderer;
extern LTexture gRulesBtcBgTexture[3];
extern LTexture gButtonBackTexture,gButtonNextTexture;
extern int windowMode;

static Button backButton,nextButton;
static int currentPage;

void initAllRulesBtc(){
	currentPage=0;
	backButton.init(86,613,gButtonBackTexture,1);
	nextButton.init(1115,613,gButtonNextTexture,1);
}

void handleEventRulesBtc(SDL_Event& e){
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

void renderAllRulesBtc(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	gRulesBtcBgTexture[currentPage].render(0,0);
	backButton.render(gButtonBackTexture,1);
	if(currentPage<2)nextButton.render(gButtonNextTexture,1);

	SDL_RenderPresent( gRenderer );
}

