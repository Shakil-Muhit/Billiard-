/* This modules controls the rendering of all pages of
 * the rules section of "Replication" mode */

#include <SDL2/SDL.h>
#include <stdio.h>
#include "include/sdl_handler.h"
#include "include/button.h"
#include "include/consts.h"

extern SDL_Renderer* gRenderer;
extern LTexture gRulesReplicationBgTexture[2];
extern LTexture gButtonBackTexture,gButtonNextTexture;
extern int windowMode;

static Button backButton,nextButton;
static int currentPage;

void initAllRulesReplication(){
	currentPage=0;
	backButton.init(86,613,gButtonBackTexture,1);
	nextButton.init(1115,613,gButtonNextTexture,1);
}

void handleEventRulesReplication(SDL_Event& e){
	if(e.type== SDL_MOUSEBUTTONDOWN && backButton.isMouseInside()){
		if(!currentPage){
			windowMode=RULES_MENU_WINDOW;
		}
		else currentPage--;
	}
	else if(e.type== SDL_MOUSEBUTTONDOWN && nextButton.isMouseInside()){
		if(!currentPage)currentPage++;
	}
}

void renderAllRulesReplication(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	gRulesReplicationBgTexture[currentPage].render(0,0);
	backButton.render(gButtonBackTexture,1);
	if(!currentPage)nextButton.render(gButtonNextTexture,1);

	SDL_RenderPresent( gRenderer );
}

