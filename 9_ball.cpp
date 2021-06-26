/* This module contains necessary functions and variables for "9 Ball Pool" mode. 
 * It controls this mode of the game */

#include "include/9_ball.h"
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

static char player1[10]="Player 1",player2[10]="Player 2",won[10]=" Won!!!";

extern Ball ball[16];
extern SDL_Renderer* gRenderer;
extern char playerName[2][10],winText[10];
extern int isTriggered;
extern int isBreakDone,placeCue,firstTouchInBreak,firstTouch,curPlayer;
extern int anyBallScored,cueBallScored,ball9Scored;
extern int cushionFoul,foulInBreak,isFoul;
extern int ballType[16],playerType[2],ballTypesScored[2],totalScored[2];
extern int isGameOver,gameWinner,gameLoser;
extern int ballsQueue[16],queueLen;

extern LTexture gBoardTexture,gBallsTexture[16],gCueTexture,gCueTriggerTexture,gBackgroundTexture[3],gTimerTexture;
extern LTexture gTextPlayerTexture[2];
extern LTexture gCueHoverTexture;
extern LTexture gLargepipeTexture,gSmallpipeTexture;
extern LTexture gButtonMenuTexture,gButtonReturnTexture;
extern LTexture gTextTipsTexture[TOTAL_TIPS];

extern LTimer timer;

static int trigger1,trigger2,magnitude=0;
extern Cue cue;
extern CueSpeedTrigger cueTrigger;
extern CueHover cueHover;
extern ThreePipes threePipes;

static Button menuButton,returnButton;

void initAll9ball(){
	isTriggered=0;
	anyBallScored=0;
	ball9Scored=0;
	for(int i=0;i<10;i++)playerName[0][i]=player1[i];
	for(int i=0;i<10;i++)playerName[1][i]=player2[i];
	for(int i=0;i<10;i++)winText[i]=won[i];
	isBreakDone=0;
	placeCue=0;
	firstTouch=0;
	firstTouchInBreak=0;
	curPlayer=0;
	cueBallScored=0;
	cushionFoul=1;
	foulInBreak=1;
	isFoul=1;
	playerType[0]=1;
	playerType[1]=1;
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
	ball[1].init( 209+EXTRA_WIDTH, 191+EXTRA_HEIGHT,1);
	ball[2].init(190+EXTRA_WIDTH,181+EXTRA_HEIGHT,2);
	ball[3].init(190+EXTRA_WIDTH,202+EXTRA_HEIGHT,3);
	ball[4].init(172-1+EXTRA_WIDTH,170+EXTRA_HEIGHT,4);
	ball[5].init(172-1+EXTRA_WIDTH,211+EXTRA_HEIGHT,5);
	ball[6].init(154-2+EXTRA_WIDTH,181+EXTRA_HEIGHT,6);
	ball[7].init(154-2+EXTRA_WIDTH,202+EXTRA_HEIGHT,7);
	ball[8].init(136-2+EXTRA_WIDTH,191+EXTRA_HEIGHT,8);
	ball[9].init(172-1+EXTRA_WIDTH,191+EXTRA_HEIGHT,9);
	ball[10].init(-100,-100,10);
	ball[11].init(-100,-100,11);
	ball[12].init(-100,-100,12);
	ball[13].init(-100,-100,13);
	ball[14].init(-100,-100,14);
	ball[15].init(-100,-100,15);

	for(int ballId=0;ballId<=9;ballId++)ballType[ballId]=ballId;

	cue.init(ball[0].x-gCueTexture.getWidth()/2,ball[0].y-gCueTexture.getHeight()/2,{ball[0].x,ball[0].y});
	cueTrigger.height=gCueTriggerTexture.getHeight();
	cueTrigger.width=gCueTriggerTexture.getWidth();
	cueTrigger.x=274+147;
	cueTrigger.y=174;


	cueHover.init(147+115,(94+gTextPlayerTexture[0].getHeight())/2,582+147,(94+gTextPlayerTexture[1].getHeight())/2);


	int largeX= (SCREEN_WIDTH-gLargepipeTexture.getWidth())/2;
	int largeY= (SCREEN_HEIGHT-EXTRA_HEIGHT_END+20);
	threePipes.init(-200,-200,-600,-600,largeX,largeY);
	menuButton.init(48,46,gButtonMenuTexture,PAUSE_MENU_WINDOW);
	returnButton.init(1115,615,gButtonReturnTexture,MAIN_MENU_WINDOW);
	timer.start();
	if( Mix_PausedMusic() == 0 )
	{
		Mix_PauseMusic();
	}
}

void handleEvent9ball(SDL_Event& e){
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

void renderAll9ball(){
	//Clear screen
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	gBackgroundTexture[0].render(0,0);
	gTimerTexture.render(147+367,72);
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

void ballScoreLogic9ball(int id){
	anyBallScored=1;
	if(id==0){
		isFoul=1;
		cueBallScored=1;
		ball[0].placeBallRandomly(ball[0],ball);
	}
	else if(id==9){
		ball9Scored=1;
	}
	else {
		ballsQueue[queueLen++]=id;
	}
}

void shotLogic9ball(){
	if(!cueBallScored&&!cushionFoul&&playerType[curPlayer]==ballType[firstTouch]){
		placeCue=1;
	}
	else {
		isFoul=1;
		if(!isBreakDone)foulInBreak=1;
	}
}

void gameWinLogic9ball(){
	if(ball[9].isInsidePocket){
		gameWinner=curPlayer;
		gameLoser=curPlayer^1;
	    if(isFoul){
			gameWinner ^= 1;
			gameLoser ^= 1;
		}
		isGameOver=1;
	}
}

void playerChangeLogic9ball(){
	int curType= playerType[curPlayer];
	if(isFoul||!anyBallScored){
		curPlayer ^= 1;
	}
}

void newShotLogic9ball(){
	isTriggered=0;
	firstTouch=0;
	cushionFoul=1;
	cueBallScored=0;
	anyBallScored=0;

	for(int ballId=1;ballId<=9;ballId++){
		if(ball[ballId].isInsidePocket)continue;
		playerType[0]=ballId;
		playerType[1]=ballId;
		break;
	}
}

int isValidAim9ball(int ballId){
	if(ballType[ballId]==playerType[curPlayer])return 1;
	return 0;
}
