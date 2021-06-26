/* This module contains necessary functions and variables for "Replication" mode. 
 * It controls this mode of the game. */

#include "include/replication_mode.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "include/consts.h"
#include "include/sdl_handler.h"
#include "include/ball.h"
#include "include/cue_trigger.h"
#include "include/cue.h"
#include "include/cue_hover.h"
#include "include/game_state.h"
#include "include/time.h"
#include "include/maths.h"
#include "include/table.h"
#include "include/window_handler.h"
#include "include/leaderboard_replication.h"
#include "include/game_end_text.h"
#include "include/button.h"
#include <stdio.h>

extern Ball ball[16];
extern SDL_Renderer* gRenderer;
extern int isTriggered;
extern int placeCue;
extern int anyBallScored,cueBallScored;
extern int cushionFoul,isFoul,foulInBreak;
extern int isGameOver;
extern int windowMode,prevWindowMode;

extern char userHandle[2][20];

extern LTexture gBoardTexture,gBallsTexture[16],gCueTexture,gCueTriggerTexture,gBackgroundTexture[3],gTimerTexture;
extern LTexture gTextPlayerTexture[2];
extern LTexture gCueHoverTexture;
extern LTexture gNameInputTexture,gInputTextTexture;
extern LTexture gButtonMenuTexture,gButtonReturnTexture;
extern LTexture gTextPlayerTexture[2],gTextCongratsTexture;
extern LTexture gTextTipsTexture[TOTAL_TIPS];

extern LTimer timer;

extern TTF_Font *gFont;

static int trigger1,trigger2,magnitude=0,ballsRevealed;
extern Cue cue;
extern CueSpeedTrigger cueTrigger;
extern CueHover cueHover;

static Button menuButton,returnButton;

static int isWon=1,isNameTaken,elapsedDuration,isLeaderboardUpdated;

static char elapsedDurationText[100];

static const int genCount=2,foulGenCount=3;

static void generateBalls(int count){
	if(isGameOver)return;
	for(int ballId=1;ballId<16;ballId++){
		if(!count)break;
		if(ball[ballId].isInsidePocket){
			ball[ballId].placeBallRandomly(ball[ballId],ball);
			count--;
		}
	}
}

void initAllReplication(){

	isTriggered=0;
	anyBallScored=0;
	placeCue=0;
	cueBallScored=0;
	cushionFoul=1;
	isFoul=1;
	foulInBreak=0;
	isGameOver=0;
	isNameTaken=0;
	isWon=1;
	ballsRevealed=8;
	magnitude=0;
	elapsedDuration=0;
	isLeaderboardUpdated=0;

	initPockets();

	initSeed();

	for(int ballId=0;ballId<9;ballId++){
		ball[ballId].init(0,0,ballId);
		ball[ballId].placeBallRandomly(ball[ballId],ball);
	}

	for(int ballId=9;ballId<16;ballId++){
		ball[ballId].init(-100,-100,ballId);
		ball[ballId].isInsidePocket=1;
	}

	for(int i=0;i<100;i++)elapsedDurationText[i]='\0';

	cue.init(ball[0].x-gCueTexture.getWidth()/2,ball[0].y-gCueTexture.getHeight()/2,{ball[0].x,ball[0].y});
	cueTrigger.height=gCueTriggerTexture.getHeight();
	cueTrigger.width=gCueTriggerTexture.getWidth();

	initLeaderboardReplication();
	menuButton.init(48,46,gButtonMenuTexture,PAUSE_MENU_WINDOW);
	returnButton.init(1115,615,gButtonReturnTexture,MAIN_MENU_WINDOW);
	timer.start();
	if( Mix_PausedMusic() == 0 )
	{
		Mix_PauseMusic();
	}
}

void handleEventReplication(SDL_Event& e){
	menuButton.handleButtonEvent(e);
	ball[0].cueBallDrag(ball[0],e,ball);

	trigger1=isTriggered;
	cue.handleEvent(e);
	cueTrigger.handleEvent(e,magnitude,0);
	trigger2=isTriggered;
	if(!trigger1&&trigger2){
		ball[0].setVectorVelocity(cue.currentDirection,magnitude);
	}

	if(isGameOver)returnButton.handleButtonEvent(e);
}

void renderAllReplication(){
	//Clear screen
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	gBackgroundTexture[0].render(0,0);
	gTimerTexture.render(147+367,72);
	gBoardTexture.render(EXTRA_WIDTH,EXTRA_HEIGHT);

	checkMovement(ball,cue,0);
	if(placeCue){
		int tmpx=ball[0].x;
		int tmpy=ball[0].y;
		ball[0].x=tmpx;
		ball[0].y=tmpy;
		cue.init(ball[0].x-gCueTexture.getWidth()/2,ball[0].y-gCueTexture.getHeight()/2,{ball[0].x,ball[0].y});
		placeCue=0;
	}

	for(int ballId=0;ballId<16;ballId++)ball[ballId].move(ball);
	cueTrigger.render();

	for(int ballId=0;ballId<16;ballId++)ball[ballId].render();

	cue.render(ball);
	loadAndRenderElapsedTime();
	if(!isWon)gameEndText(isWon);
	else if(isWon && !isEligibleLeaderboardReplication(elapsedDuration))gameEndText(isWon);

	if(isGameOver && isWon && isEligibleLeaderboardReplication(elapsedDuration)){
		if(!isLeaderboardUpdated){
			updateLeaderboardReplication(userHandle[0],elapsedDurationText,elapsedDuration);
			isLeaderboardUpdated=1;
		}
	}
	menuButton.render(gButtonMenuTexture,0);
	gTextPlayerTexture[0].render(1180-gTextPlayerTexture[0].getWidth(),15);
	if(isFoul){
		gTextTipsTexture[T_DRAG].render((SCREEN_WIDTH-gTextTipsTexture[T_DRAG].getWidth())/2, 30);
	}
	if(isGameOver && isWon && isEligibleLeaderboardReplication(elapsedDuration)){
		int width= gTextCongratsTexture.getWidth();
		int height= gTextCongratsTexture.getHeight();
		gTextCongratsTexture.render((SCREEN_WIDTH-width)/2,(SCREEN_HEIGHT-height)/2);
	}
	if(isGameOver)returnButton.render(gButtonReturnTexture,1);
	//Update screen
	SDL_RenderPresent( gRenderer );

}

void ballScoreLogicReplication(int id){
	if(id==0){
		isFoul=1;
		cueBallScored=1;
		ball[0].placeBallRandomly(ball[0],ball);
	}
	else anyBallScored=1;
}

void shotLogicReplication(){
	if(isGameOver)return;
	if(!cueBallScored&&!cushionFoul){
		placeCue=1;
	}
	else {
		isFoul=1;
	}
}

void gameWinLogicReplication(){
	if(ballsRevealed>15){
		isGameOver=1;
		isWon=0;
		elapsedDuration=timer.getTicks();
		formatTime(elapsedDurationText,elapsedDuration);
		zeroPadTime(elapsedDurationText);
	}
	if(!ballsRevealed){
		isGameOver=1;
		isWon=1;
		elapsedDuration=timer.getTicks();
		formatTime(elapsedDurationText,elapsedDuration);
		zeroPadTime(elapsedDurationText);
	}
}

void newShotLogicReplication(){
	isTriggered=0;
	cushionFoul=1;
	cueBallScored=0;
	ballsRevealed=0;
	for(int ballId=1;ballId<16;ballId++){
		if(!ball[ballId].isInsidePocket)ballsRevealed++;
	}
	if(isFoul){
		ballsRevealed += foulGenCount;
		generateBalls(foulGenCount);
	}
	else if(!anyBallScored){
		ballsRevealed+=genCount;
		generateBalls(genCount);
	}
	anyBallScored=0;
	gameWinLogicReplication();
}

int isValidAimReplication(int ballId){
	return 1;
}
