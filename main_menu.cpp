/* This module controls the main menu of the game and directs
 * to play,help,leaderboard and exit button */

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include "include/sdl_handler.h"
#include "include/button.h"
#include "include/consts.h"

extern LTexture gMenu1BackgroundTexture,gButtonPlayTexture,gButtonHelpTexture;
extern LTexture gButtonLeaderboardTexture,gButtonExitTexture;
extern SDL_Renderer* gRenderer;
extern LTexture gCueHoverTexture;
extern int windowMode,isLoggedIn;

extern Mix_Music *gBackgroundMusic;

static int hoverX,hoverY,renderFlag=0;

static Button playButton,exitButton,helpButton,leaderboardButton;

void initAllMainMenu(){
	int playTargetWindow= PLAY_MENU_WINDOW;
	if(!isLoggedIn)playTargetWindow= LOGIN_MENU_WINDOW;
	playButton.init(151,151,gButtonPlayTexture,playTargetWindow);
	helpButton.init(430,351,gButtonHelpTexture,HELP_MENU_WINDOW);
	exitButton.init(135,512,gButtonExitTexture,QUIT_GAME);
	leaderboardButton.init(674,151,gButtonLeaderboardTexture,LEADERBOARD_WINDOW);
	if(Mix_PausedMusic())Mix_ResumeMusic();
	//Play the music
    else Mix_PlayMusic( gBackgroundMusic, -1 );
}

void handleEventMainMenu(SDL_Event& e){
	playButton.handleButtonEvent(e);
	leaderboardButton.handleButtonEvent(e);
	helpButton.handleButtonEvent(e);
	exitButton.handleButtonEvent(e);
}

void renderAllMainMenu(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	gMenu1BackgroundTexture.render(0,0);
	playButton.render(gButtonPlayTexture,1);
	helpButton.render(gButtonHelpTexture,1);
	exitButton.render(gButtonExitTexture,1);
	leaderboardButton.render(gButtonLeaderboardTexture,1);

	SDL_RenderPresent( gRenderer );
}
