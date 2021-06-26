/* This module controls the rules section and directs to
 * rules to all the modes */

#include <SDL2/SDL.h>
#include <stdio.h>
#include "include/sdl_handler.h"
#include "include/button.h"
#include "include/consts.h"

extern SDL_Renderer* gRenderer;

extern LTexture gPlayMenuBackgroundTexture;
extern LTexture gButton8BallTexture,gButton9BallTexture,gButtonBackTexture;
extern LTexture gButtonReplicationTexture,gButtonBtcTexture;

static Button mode8ballButton,mode9ballButton,backButton;
static Button modeReplicationButton,modeBtcButton;

void initAllRulesMenu(){
	mode8ballButton.init(218,152,gButton8BallTexture,RULES_8BALL_MENU_WINDOW);
	mode9ballButton.init(1027,121,gButton9BallTexture,RULES_9BALL_MENU_WINDOW);
	modeReplicationButton.init(924,481,gButtonReplicationTexture,RULES_REPLICATION_MENU_WINDOW);
	modeBtcButton.init(617,137,gButtonBtcTexture,RULES_BTC_MENU_WINDOW);
	backButton.init(86,613,gButtonBackTexture,HELP_MENU_WINDOW);
}

void handleEventRulesMenu(SDL_Event& e){
	mode8ballButton.handleButtonEvent(e);
	mode9ballButton.handleButtonEvent(e);
	modeReplicationButton.handleButtonEvent(e);
	modeBtcButton.handleButtonEvent(e);
	backButton.handleButtonEvent(e);
}

void renderAllRulesMenu(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	gPlayMenuBackgroundTexture.render(0,0);
	mode8ballButton.render(gButton8BallTexture,1);
	mode9ballButton.render(gButton9BallTexture,1);
	modeReplicationButton.render(gButtonReplicationTexture,1);
	backButton.render(gButtonBackTexture,1);
	modeBtcButton.render(gButtonBtcTexture,1);

	SDL_RenderPresent( gRenderer );
}
