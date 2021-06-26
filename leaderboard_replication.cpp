/* This module controls the leaderboard of "Replication" mode.
 * Reads and updates leaderboard_replication.bin */

#include <SDL2/SDL.h>
#include <stdio.h>
#include "include/sdl_handler.h"
#include "include/button.h"
#include "include/consts.h"
#include "include/record.h"

extern Record recData[LEADERBOARD_SIZE];

extern LTexture gLeaderboardReplicationTexture;
extern LTexture gButtonBackTexture,gButtonReturnTexture;

extern SDL_Renderer* gRenderer;

static Button backButton,returnButton;
static char filePath[]="bin/leaderboard_replication.bin";

void initLeaderboardReplication(){
	initRecord(filePath,1000000000);

    backButton.init(91,611,gButtonBackTexture,LEADERBOARD_WINDOW);
    returnButton.init(1100,611,gButtonReturnTexture,MAIN_MENU_WINDOW);
}

int isEligibleLeaderboardReplication(int ms){
	if(recData[LEADERBOARD_SIZE-1].duration>ms)return 1;
	return 0;
}

void updateLeaderboardReplication(char name[],char durationText[],int duration){
	if(!isEligibleLeaderboardReplication(duration))return;

	int pos=0;
	for(int recId=0;recId<LEADERBOARD_SIZE;recId++){
		if(recData[recId].duration>duration){
			pos=recId;
			break;
		}
	}
	insertRecord(pos,name,durationText,duration);

	writeData(filePath);

}

void handleEventLeaderboardReplication(SDL_Event &e){
	backButton.handleButtonEvent(e);
	returnButton.handleButtonEvent(e);
}

void renderLeaderboardReplication(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	gLeaderboardReplicationTexture.render(0,0);

	renderRecord();

	backButton.render(gButtonBackTexture,1);
	returnButton.render(gButtonReturnTexture,1);

	SDL_RenderPresent( gRenderer );
	//write_data();
}
