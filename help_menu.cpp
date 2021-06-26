/* This module controls the "Help" section of the game and directs
 * to rules,introduction and controls*/

#include <SDL2/SDL.h>
#include <stdio.h>
#include "include/sdl_handler.h"
#include "include/button.h"
#include "include/consts.h"

extern SDL_Renderer* gRenderer;

extern LTexture gLeaderboardBackgroundTexture;
extern LTexture gButtonIntroductionTexture,gButtonRulesTexture;
extern LTexture gButtonControlsTexture,gButtonBackTexture;

extern int prevWindowMode;

static Button backButton,rulesButton;
static Button introductionButton,controlsButton;
static int prev;

void initAllHelpMenu(){
	if(prevWindowMode==MAIN_MENU_WINDOW || prevWindowMode==PAUSE_MENU_WINDOW){
		prev=prevWindowMode;
	}
	introductionButton.init(250,267,gButtonIntroductionTexture,INTRODUCTION_MENU_WINDOW);
	controlsButton.init(730,232,gButtonControlsTexture,CONTROLS_MENU_WINDOW);
	rulesButton.init(933,525,gButtonRulesTexture,RULES_MENU_WINDOW);
	backButton.init(86,613,gButtonBackTexture,prev);
}

void handleEventHelpMenu(SDL_Event& e){
	introductionButton.handleButtonEvent(e);
	controlsButton.handleButtonEvent(e);
	rulesButton.handleButtonEvent(e);
	backButton.handleButtonEvent(e);
}

void renderAllHelpMenu(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	gLeaderboardBackgroundTexture.render(0,0);
	introductionButton.render(gButtonIntroductionTexture,1);
	controlsButton.render(gButtonControlsTexture,1);
	rulesButton.render(gButtonRulesTexture,1);
	backButton.render(gButtonBackTexture,1);

	SDL_RenderPresent( gRenderer );
}
