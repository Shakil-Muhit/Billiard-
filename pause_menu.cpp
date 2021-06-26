/*This module controls the pause menu of all the modes */

#include <SDL2/SDL.h>
#include "include/sdl_handler.h"
#include "include/button.h"
#include "include/consts.h"

extern LTexture gPauseBackgroundTexture;
extern LTexture gButtonReturnGameTexture,gButtonHelpTexture;
extern LTexture gButtonQuitTexture,gButtonRestartTexture;
extern LTexture gButtonChangeSidesTexture;
extern LTimer timer;
extern SDL_Renderer* gRenderer;

extern int prevWindowMode,windowMode;

static Button restartButton,quitButton,changeSidesButton;
static Button returnGameButton,helpButton;
static int prev;

void initAllPauseMenu(){
	if(prevWindowMode!=LOGIN_MENU_WINDOW && prevWindowMode!=HELP_MENU_WINDOW){
		prev=prevWindowMode;
	}
	timer.pause();
	returnGameButton.init(170,527,gButtonReturnGameTexture,prevWindowMode);
	restartButton.init(352,308,gButtonRestartTexture,prevWindowMode);
	helpButton.init(576,527,gButtonHelpTexture,HELP_MENU_WINDOW);
	quitButton.init(1024,527,gButtonQuitTexture,MAIN_MENU_WINDOW);
	changeSidesButton.init(800,308,gButtonChangeSidesTexture,LOGIN_MENU_WINDOW);
}

void handleEventPauseMenu(SDL_Event& e){
	quitButton.handleButtonEvent(e);
	restartButton.handleButtonEvent(e);
	changeSidesButton.handleButtonEvent(e);
	helpButton.handleButtonEvent(e);

	if(e.type== SDL_MOUSEBUTTONDOWN && returnGameButton.isMouseInside()){
		if(prev==MODE_8BALL_WINDOW){
			windowMode= MODE_8BALL_WINDOW;
			prevWindowMode= MODE_8BALL_WINDOW;
			timer.unpause();
		}
		else if(prev==MODE_9BALL_WINDOW){
			windowMode= MODE_9BALL_WINDOW;
			prevWindowMode= MODE_9BALL_WINDOW;
			timer.unpause();
		}
		else if(prev==MODE_REPLICATION_WINDOW){
			windowMode= MODE_REPLICATION_WINDOW;
			prevWindowMode= MODE_REPLICATION_WINDOW;
			timer.unpause();
		}
		else if(prev==MODE_BTC_WINDOW){
			windowMode= MODE_BTC_WINDOW;
			prevWindowMode= MODE_BTC_WINDOW;
			timer.unpause();
		}
	}
}

void renderAllPauseMenu(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	gPauseBackgroundTexture.render(0,0);
	quitButton.render(gButtonQuitTexture,1);
	helpButton.render(gButtonHelpTexture,1);
	restartButton.render(gButtonRestartTexture,1);
	returnGameButton.render(gButtonReturnGameTexture,1);
	changeSidesButton.render(gButtonChangeSidesTexture,1);

	SDL_RenderPresent( gRenderer );
}
