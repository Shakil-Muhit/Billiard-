/* This module contains necessary functions and variables for "Beat the Clock" mode. 
 * It controls this mode of the game */

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
#include "include/leaderboard_btc.h"
//#include "include/input_text.h"
#include "include/game_end_text.h"
#include "include/modified_ball.h"
#include "include/button.h"
#include "include/string.h"
#include <stdio.h>

static const int SCORE_LIMIT=2,MISS_LIMIT=2;

extern Ball ball[16];
extern ModifiedBall modifiedBall[16];
extern SDL_Renderer* gRenderer;
extern int isTriggered;
extern int placeCue;
extern int anyBallScored,cueBallScored;
extern int cushionFoul,isFoul,foulInBreak;
extern int isGameOver;
extern int windowMode,prevWindowMode;

extern char inputText[100];

extern LTexture gBoardTexture,gBallsTexture[16],gCueTexture,gCueTriggerTexture,gBackgroundTexture[3],gTimerTexture;
extern LTexture gModifiedBallsTexture[16];
extern LTexture gNameInputTexture,gInputTextTexture;
extern LTexture gButtonMenuTexture,gButtonReturnTexture;
extern LTexture gTextPlayerTexture[2],gTextCongratsTexture;
extern LTexture gTextTipsTexture[TOTAL_TIPS];
extern LTexture gTextPenaltytexture,gTextRewardtexture;

extern LTimer timer;

extern TTF_Font *gFont;

static int trigger1,trigger2,magnitude=0,consecutiveScore,consecutiveMiss;
extern Cue cue;
extern CueSpeedTrigger cueTrigger;
extern CueHover cueHover;

extern char userHandle[2][20];

static Button menuButton,returnButton;

static const int MAX_SHOW_TIME=300;

static int isWon=1,isNameTaken,elapsedDuration,totalDuration,isLeaderboardUpdated;
static int showEffectSpan,curPenaltyBall,curRewardBall;
static char durationLeft[100];

static void applyPenalty(int points){
	if(isGameOver)return;
	int candidates[16],len=0;
	for(int id=1;id<16;id++){
		if(!ball[id].isInsidePocket)candidates[len++]=id;
	}
	if(!len)return;

	int ballId=getRandom(0,len-1);
	modifiedBall[candidates[ballId]].points += points;
	showEffectSpan=MAX_SHOW_TIME;
	if(points<0){
		curPenaltyBall=candidates[ballId];
		curRewardBall=0;
	}
	else {
		curRewardBall=candidates[ballId];
		curPenaltyBall=0;
	}
}

void initAllBtc(){
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
	//balls_revealed=8;
	magnitude=0;
	elapsedDuration=0;
	consecutiveMiss=0;
	consecutiveScore=0;
	totalDuration=BTC_INITIAL_TIME;
	isLeaderboardUpdated=0;
	showEffectSpan=0;
	curPenaltyBall=curRewardBall=0;

	initPockets();

	//ball[0].init( gBallsTexture[0].getWidth() / 2+500, 191+EXTRA_HEIGHT,0);
	initSeed();

	for(int ballId=0;ballId<16;ballId++){
		ball[ballId].init(0,0,ballId);
		ball[ballId].placeBallRandomly(ball[ballId],ball);
	}

	for(int i=0;i<100;i++)durationLeft[i]='\0';

	cue.init(ball[0].x-gCueTexture.getWidth()/2,ball[0].y-gCueTexture.getHeight()/2,{ball[0].x,ball[0].y});
	cueTrigger.height=gCueTriggerTexture.getHeight();
	cueTrigger.width=gCueTriggerTexture.getWidth();

	//cue_hover.init(115,(94+gTextPlayerTexture[0].getHeight())/2,582,(94+gTextPlayerTexture[1].getHeight())/2);
	//init_leaderboard_replication();
	initLeaderboardBtc();
	menuButton.init(48,46,gButtonMenuTexture,PAUSE_MENU_WINDOW);
	returnButton.init(1115,615,gButtonReturnTexture,MAIN_MENU_WINDOW);

	timer.start();
	//init_text_input();
	initModifiedBall();
	if( Mix_PausedMusic() == 0 )
	{
		Mix_PauseMusic();
	}
}

void handleEventBtc(SDL_Event& e){
	menuButton.handleButtonEvent(e);
	ball[0].cueBallDrag(ball[0],e,ball);

	trigger1=isTriggered;
	cue.handleEvent(e);
	cueTrigger.handleEvent(e,magnitude,0);
	trigger2=isTriggered;
	if(!trigger1&&trigger2){
		ball[0].setVectorVelocity(cue.currentDirection,magnitude);                        //printf("MAGNITUDE : %d\n",magnitude);
	}
	//if(is_game_over&&is_won&&!is_name_taken&&is_eligible_leaderboard_btc(elapsed_duration)){
		//is_name_taken=handle_event_text_input(e);
		//SDL_Color textColor = { 0, 0, 0, 0xFF };
		//load_text_input(textColor);
	//}
	handleEventModifiedBall(e);
	if(isGameOver)returnButton.handleButtonEvent(e);
}

void gameWinLogicBtc(){
	if(isGameOver)return;

	if(timer.getTicks()>totalDuration){
		isGameOver=1;
		isWon=0;
		return;
	}

	int finished=1;
	for(int ballId=1;ballId<16;ballId++){
		if(!ball[ballId].isInsidePocket)finished=0;
	}

	if(finished){
		isGameOver=1;
		isWon=1;
		elapsedDuration=timer.getTicks();
		//format_time(duration_left,total_duration-elapsed_duration);
		toString((totalDuration-elapsedDuration)/1000,durationLeft);
	}
}

void renderAllBtc(){
	//Clear screen
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	gBackgroundTexture[0].render(0,0);
	gTimerTexture.render(367+147,72);
	gBoardTexture.render(EXTRA_WIDTH,EXTRA_HEIGHT);

	gameWinLogicBtc();
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


	loadAndRenderRemainingTime(totalDuration);
	if(!isWon)gameEndText(isWon);
	else if(isWon && !isEligibleLeaderboardBtc(elapsedDuration))gameEndText(isWon);

	if(isGameOver && isWon && isEligibleLeaderboardBtc(elapsedDuration)){
		//gNameInputTexture.render((SCREEN_WIDTH-gNameInputTexture.getWidth()+50)/2,(SCREEN_HEIGHT-gNameInputTexture.getHeight())/2);
		//gInputTextTexture.render( ( SCREEN_WIDTH - gInputTextTexture.getWidth() ) / 2, SCREEN_HEIGHT/2 );
		if(!isLeaderboardUpdated){
			//prev_window_mode=-1;
			//window_mode=MAIN_MENU_WINDOW;
			updateLeaderboardBtc(userHandle[0],durationLeft,totalDuration-elapsedDuration);
			isLeaderboardUpdated=1;
		}
	}
	//render_time();
	//declare_winner();
	menuButton.render(gButtonMenuTexture,0);
	gTextPlayerTexture[0].render(1180-gTextPlayerTexture[0].getWidth(),15);
	//Update screen
	renderModifiedPipe();
	if(isFoul){
		gTextTipsTexture[T_DRAG].render((SCREEN_WIDTH-gTextTipsTexture[T_DRAG].getWidth())/2, 30);
	}
	else {
		gTextTipsTexture[T_TOGGLE].render((SCREEN_WIDTH-gTextTipsTexture[T_TOGGLE].getWidth())/2, 30);
	}
	cue.render(ball);
	if(isGameOver && isWon && isEligibleLeaderboardBtc(elapsedDuration)){
		int width= gTextCongratsTexture.getWidth();
		int height= gTextCongratsTexture.getHeight();
		gTextCongratsTexture.render((SCREEN_WIDTH-width)/2,(SCREEN_HEIGHT-height)/2);
	}
	if(isGameOver)returnButton.render(gButtonReturnTexture,1);
	if(showEffectSpan){
		int x1= 1055,y1=60;
		if(curPenaltyBall){
			int w1= gModifiedBallsTexture[curPenaltyBall].getWidth();
			gModifiedBallsTexture[curPenaltyBall].render(x1,y1);
			gTextPenaltytexture.render(x1+w1+10,y1);
		}
		else if(curRewardBall){
			int w1= gModifiedBallsTexture[curRewardBall].getWidth();
			gModifiedBallsTexture[curRewardBall].render(x1-5,y1);
			gTextRewardtexture.render(x1+w1+5,y1);
		}
		showEffectSpan--;
	}
	//int x1= 1055,y1=60;
	//int w1= gModifiedBallsTexture[2].getWidth();
			//gModifiedBallsTexture[2].render(x1,y1);
			//gTextPenaltytexture.render(x1+w1+10,y1);

	SDL_RenderPresent( gRenderer );
}

void ballScoreLogicBtc(int id){
	if(id==0){
		isFoul=1;
		cueBallScored=1;
		ball[0].placeBallRandomly(ball[0],ball);
	}
	else {
		anyBallScored=1;
		totalDuration += modifiedBall[id].points*1000;
	}
}

void shotLogicBtc(){
	if(isGameOver)return;
	if(!cueBallScored&&!cushionFoul){
		placeCue=1;
	}
	else {
		isFoul=1;
	}
}

void newShotLogicBtc(){
	isTriggered=0;
	cushionFoul=1;
	cueBallScored=0;

	if(isFoul){
		applyPenalty(-10);
	}

	if(!anyBallScored){
		consecutiveMiss++;
		consecutiveScore=0;
		if(consecutiveMiss>MISS_LIMIT){
			consecutiveMiss=0;
			applyPenalty(-10);
		}
	}
	else {
		consecutiveScore++;
		consecutiveMiss=0;
		if(consecutiveScore>SCORE_LIMIT){
			consecutiveScore=0;
			applyPenalty(10);
		}
	}
	//printf("%d %d\n",consecutive_miss,consecutive_score);
	anyBallScored=0;
	gameWinLogicBtc();
}

int isValidAimBtc(int ballId){
	return 1;
}
