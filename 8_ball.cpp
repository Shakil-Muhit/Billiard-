/* This module contains necessary functions and variables for "8 Ball Pool" mode. 
 * It controls this mode of the game. */

#include "include/8_ball.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "include/consts.h"
#include "include/sdl_handler.h"
#include "include/ball.h"
#include "include/cue_trigger.h"
#include "include/cue.h"
#include "include/cue_hover.h"
#include "include/pipes.h"
#include "include/game_state.h"
#include "include/time.h"
#include "include/table.h"
#include "include/button.h"

extern TTF_Font *gFont;

extern Ball ball[16];
extern SDL_Renderer* gRenderer;
extern char playerName[2][10],winText[10];
extern int isTriggered;
extern int isBreakDone,placeCue,firstTouchInBreak,firstTouch,curPlayer;
extern int anyBallScored,cueBallScored;
extern int cushionFoul,foulInBreak,isFoul;
extern int ballType[16],playerType[2],ballTypesScored[2],totalScored[2];
extern int isGameOver,gameWinner,gameLoser;
extern int ballsQueue[16],queueLen;
extern int prevPlayerType[2];

extern LTexture gBoardTexture,gBallsTexture[16],gCueTexture,gCueTriggerTexture,gBackgroundTexture[3],gTimerTexture;
extern LTexture gTextPlayerTexture[2];
extern LTexture gCueHoverTexture;
extern LTexture gLargepipeTexture,gSmallpipeTexture;
extern LTexture gButtonMenuTexture,gButtonReturnTexture;
extern LTexture gTextTipsTexture[TOTAL_TIPS];

extern LTimer timer;

static int trigger1,trigger2,magnitude=0;
Cue cue;
CueSpeedTrigger cueTrigger(147+274,174);
CueHover cueHover;
ThreePipes threePipes;

static Button menuButton,returnButton;

void initAll8ball(){
	isTriggered=0;
	//player_name[0]="Player 1";
	//player_name[1]="Player 2";
	//for(int i=0;i<10;i++)player_name[0][i]=player1[i];
	//for(int i=0;i<10;i++)player_name[1][i]=player2[i];
	//for(int i=0;i<10;i++)win_text[i]=won[i];
	anyBallScored=0;
	isBreakDone=0;
	placeCue=0;
	firstTouch=0;
	firstTouchInBreak=0;
	curPlayer=0;
	cueBallScored=0;
	cushionFoul=1;
	foulInBreak=1;
	isFoul=1;
	playerType[0]=-1;
	playerType[1]=-1;
	prevPlayerType[0]=-1;
	prevPlayerType[1]=-1;
	ballTypesScored[0]=0;
	ballTypesScored[1]=0;
	totalScored[0]=0;
	totalScored[1]=0;
	isGameOver=0;
	gameWinner=-1;
	gameLoser=-1;
	queueLen=0;

	initPockets();

	ball[0].init( gBallsTexture[0].getWidth() / 2+500, 191+EXTRA_HEIGHT,0);
	ball[1].init( 225+1+EXTRA_WIDTH, 191+EXTRA_HEIGHT,1);
	ball[2].init(207+EXTRA_WIDTH,201+1+EXTRA_HEIGHT,2);
	ball[3].init(189-1+EXTRA_WIDTH,214+EXTRA_HEIGHT,3);
	ball[4].init(171-1-2+EXTRA_WIDTH,224+EXTRA_HEIGHT,4);
	ball[5].init(151-1-4+EXTRA_WIDTH,149+EXTRA_HEIGHT,5);
	ball[6].init(152-1-1-4+EXTRA_WIDTH,212+1+EXTRA_HEIGHT,6);
	ball[7].init(170-1-2+EXTRA_WIDTH,182+EXTRA_HEIGHT,7);
	ball[8].init(189-1+EXTRA_WIDTH,192+EXTRA_HEIGHT,8);
	ball[9].init(207+EXTRA_WIDTH,181-1+EXTRA_HEIGHT,9);
	ball[10].init(189-1+EXTRA_WIDTH,170+EXTRA_HEIGHT,10);
	ball[11].init(170-1-2+EXTRA_WIDTH,160+EXTRA_HEIGHT,11);
	ball[12].init(152-1-4+EXTRA_WIDTH,233+1+EXTRA_HEIGHT,12);
	ball[13].init(151-1-4+EXTRA_WIDTH,171+EXTRA_HEIGHT,13);
	ball[14].init(170-1-2+EXTRA_WIDTH,202+1+EXTRA_HEIGHT,14);
	ball[15].init(151-1-4+EXTRA_WIDTH,192+EXTRA_HEIGHT,15);

	ballType[0]=ballType[8]=-2;
	for(int ballId=1;ballId<8;ballId++)ballType[ballId]=0;
	for(int ballId=9;ballId<16;ballId++)ballType[ballId]=1;


	cue.init(ball[0].x-gCueTexture.getWidth()/2,ball[0].y-gCueTexture.getHeight()/2,{ball[0].x,ball[0].y});
	cueTrigger.height=gCueTriggerTexture.getHeight();
	cueTrigger.width=gCueTriggerTexture.getWidth();



	cueHover.init(147+115,(94+gTextPlayerTexture[0].getHeight())/2,147+582,(94+gTextPlayerTexture[1].getHeight())/2);


	int largeX= (SCREEN_WIDTH-gLargepipeTexture.getWidth())/2;
	int largeY= (SCREEN_HEIGHT-EXTRA_HEIGHT_END+20);
	threePipes.init(147+115,94,147+582,94,largeX,largeY);
	timer.start();
	menuButton.init(48,46,gButtonMenuTexture,PAUSE_MENU_WINDOW);
	returnButton.init(1115,615,gButtonReturnTexture,MAIN_MENU_WINDOW);
	if( Mix_PausedMusic() == 0 )
	{
		Mix_PauseMusic();
	}

}

void handleEvent8ball(SDL_Event& e){
	menuButton.handleButtonEvent(e);
	ball[0].cueBallDrag(ball[0],e,ball);

	trigger1=isTriggered;
	cue.handleEvent(e);
	cueTrigger.handleEvent(e,magnitude,1);
	trigger2=isTriggered;
	if(!trigger1&&trigger2){
		ball[0].setVectorVelocity(cue.currentDirection,magnitude);                        //printf("MAGNITUDE : %d\n",magnitude);
	}
	if(isGameOver)returnButton.handleButtonEvent(e);
}

void renderAll8ball(){
	//Clear screen
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	gBackgroundTexture[0].render(0,0);
	gTimerTexture.render(367+147,72);
	gBoardTexture.render(EXTRA_WIDTH,EXTRA_HEIGHT);
	gTextPlayerTexture[0].render(147+115+(gSmallpipeTexture.getWidth()-gTextPlayerTexture[0].getWidth())/2,(94-gTextPlayerTexture[0].getHeight())/2);
	gTextPlayerTexture[1].render(147+582+(gSmallpipeTexture.getWidth()-gTextPlayerTexture[1].getWidth())/2,(94-gTextPlayerTexture[1].getHeight())/2);
	cueHover.render();
	threePipes.render();

	checkMovement(ball,cue,1);
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
	renderTime();
	declareWinner();
	menuButton.render(gButtonMenuTexture,0);
	if(isFoul){
		gTextTipsTexture[T_DRAG].render((SCREEN_WIDTH-gTextTipsTexture[T_DRAG].getWidth())/2, SCREEN_HEIGHT-40);
	}
	if(isGameOver)returnButton.render(gButtonReturnTexture,1);
	//Update screen
	SDL_RenderPresent( gRenderer );

}

void ballScoreLogic8ball(int id){
	int curBallType=ballType[id];
	if(id==0){
		isFoul=1;
		cueBallScored=1;
		ball[0].placeBallRandomly(ball[0],ball);
	}
	else if(id==8){
		int tp=playerType[curPlayer];
		if(totalScored[tp]!=7)gameLoser=curPlayer;
		ballsQueue[queueLen++]=id;
		//else game_winner=cur_player^1;
	}
	else {
		if(isBreakDone&&playerType[curPlayer]==-1){
			playerType[curPlayer]=curBallType;
			playerType[curPlayer^1]=(curBallType^1);
		}
		ballTypesScored[ballType[id]]=1;
		totalScored[ballType[id]]++;
		if(playerType[0]==-1)ballTypesScored[ballType[id]^1]=1;
		ballsQueue[queueLen++]=id;
	}
}

void shotLogic8ball(){
	if(!cueBallScored&&!cushionFoul&&(prevPlayerType[0]==-1||playerType[curPlayer]==ballType[firstTouch])){
		placeCue=1;
	}
	else if(playerType[0]!=-1&&firstTouch==8&&totalScored[playerType[curPlayer]]==7)placeCue=1;
	else {
		isFoul=1;
		if(!isBreakDone)foulInBreak=1;
	}
	prevPlayerType[0]= playerType[0];
	prevPlayerType[1]= playerType[1];
}

void gameWinLogic8ball(){
	if(ball[8].isInsidePocket){
		if(gameLoser!=-1){
			gameWinner=gameLoser^1;
		}
		else {
			if(firstTouch!=8||cueBallScored)gameWinner=curPlayer^1;
			else gameWinner=curPlayer;
		}
		isGameOver=1;
	}
}

void playerChangeLogic8ball(){
	int curType= playerType[curPlayer];
	if(isFoul||(!isBreakDone&&!totalScored[0]&&!totalScored[1])||(isBreakDone&&playerType[0]==-1)||!ballTypesScored[curType]){
		curPlayer ^= 1;
	}
}

void newShotLogic8ball(){
	isTriggered=0;
	firstTouch=0;
	ballTypesScored[0]=0;
	ballTypesScored[1]=0;
	cushionFoul=1;
	cueBallScored=0;
}

int isValidAim8ball(int ballId){
	if(ballId!=8){
		if(playerType[0]==-1||ballType[ballId]==playerType[curPlayer])return 1;
	}
	else {
		if(playerType[0]!=-1&&totalScored[playerType[curPlayer]]==7)return 1;
	}
	return 0;
}
