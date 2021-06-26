/* This module controls the play section and directs to 
 * all the four modes of games */

#include <SDL2/SDL.h>
#include <stdio.h>
#include "include/sdl_handler.h"
#include "include/button.h"
#include "include/consts.h"

extern LTexture gPlayMenuBackgroundTexture,gButton8BallTexture;
extern LTexture gButton9BallTexture,gButtonReplicationTexture,gButtonBackTexture;
extern LTexture gButtonBtcTexture;
extern SDL_Renderer* gRenderer;
extern LTexture gCueHoverTexture;
extern int windowMode;

static int hoverX,hoverY,renderFlag=0;

static Button mode8ballButton,mode9ballButton,modeReplicationButton,backButton;
static Button modeBtcButton;

void initAllPlayMenu(){
	mode8ballButton.init(218,152,gButton8BallTexture,MODE_8BALL_WINDOW);
	mode9ballButton.init(1027,121,gButton9BallTexture,MODE_9BALL_WINDOW);
	modeReplicationButton.init(924,481,gButtonReplicationTexture,MODE_REPLICATION_WINDOW);
	modeBtcButton.init(617,137,gButtonBtcTexture,MODE_BTC_WINDOW);
	backButton.init(86,613,gButtonBackTexture,MAIN_MENU_WINDOW);
}

void handleEventPlayMenu(SDL_Event& e){
	mode8ballButton.handleButtonEvent(e);
	mode9ballButton.handleButtonEvent(e);
	modeReplicationButton.handleButtonEvent(e);
	modeBtcButton.handleButtonEvent(e);
	backButton.handleButtonEvent(e);
}

void renderAllPlayMenu(){
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

