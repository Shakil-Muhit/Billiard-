/* This module controls the leaderboard of "Beat the Clock" mode.
 * Reads and updates leaderboard_btc.bin */

#include <SDL2/SDL.h>
#include <stdio.h>
#include "include/sdl_handler.h"
#include "include/button.h"
#include "include/consts.h"
#include "include/record.h"

extern Record recData[LEADERBOARD_SIZE];

extern LTexture gLeaderboardBtcTexture;
extern LTexture gButtonBackTexture,gButtonReturnTexture;

extern SDL_Renderer* gRenderer;

static Button backButton,returnButton;
static char filePath[]="bin/leaderboard_btc.bin";

void initLeaderboardBtc(){
	initRecord(filePath,-100000000);

    backButton.init(91,611,gButtonBackTexture,LEADERBOARD_WINDOW);
    returnButton.init(1100,611,gButtonReturnTexture,MAIN_MENU_WINDOW);
}

int isEligibleLeaderboardBtc(int ms){
	if(recData[LEADERBOARD_SIZE-1].duration<ms)return 1;
	return 0;
}

void updateLeaderboardBtc(char name[],char durationText[],int duration){
	if(!isEligibleLeaderboardBtc(duration))return;

	int pos=0;
	for(int recId=0;recId<LEADERBOARD_SIZE;recId++){
		if(recData[recId].duration<duration){
			pos=recId;
			break;
		}
	}

	insertRecord(pos,name,durationText,duration);

	writeData(filePath);
}

void handleEventLeaderboardBtc(SDL_Event &e){
	backButton.handleButtonEvent(e);
	returnButton.handleButtonEvent(e);
}

void renderLeaderboardBtc(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	gLeaderboardBtcTexture.render(0,0);

	renderRecord();

	backButton.render(gButtonBackTexture,1);
	returnButton.render(gButtonReturnTexture,1);

	SDL_RenderPresent( gRenderer );
	//write_data();
}
