/* This module controls various in-game states like balls movement
 * and merges the required effects 
 * Also renders the winner declaration text in two player modes */

#include "include/cue.h"
#include "include/ball.h"
#include "include/sdl_handler.h"
#include "include/consts.h"
#include "include/game_state.h"
#include "include/window_handler.h"
#include "include/8_ball.h"
#include "include/9_ball.h"
#include "include/replication_mode.h"
#include "include/btc_mode.h"
#include "include/string.h"

extern LTexture gTextPlayerTexture[2],gTextWinTexture;
extern int isGameOver,isTriggered,isFoul,cueBallScored,curPlayer;
extern int foulInBreak,cushionFoul,playerType[2],placeCue,firstTouch;
extern int totalScored[2],isBreakDone,gameLoser,gameWinner;
extern int ballTypesScored[2],ballType[16];
extern LTimer timer;
extern int windowMode;
extern char userHandle[2][20];

extern TTF_Font *gFontResult;

void checkMovement(Ball ball[],Cue &cue,int restartTimeFlag){
    if(isGameOver)return;
    int trigger=0;
    for(int i=0;i<16;i++){
        if(!ball[i].isInsidePocket&&ball[i].isMoving)trigger=1;
    }
    if(trigger)isTriggered=1;
    else {
        if(isTriggered){

            if(windowMode==MODE_8BALL_WINDOW){
				shotLogic8ball();
				gameWinLogic8ball();
				playerChangeLogic8ball();
			}
            else if(windowMode==MODE_9BALL_WINDOW){
				shotLogic9ball();
				gameWinLogic9ball();
				playerChangeLogic9ball();
			}
			else if(windowMode==MODE_REPLICATION_WINDOW){
				shotLogicReplication();
				gameWinLogicReplication();
			}
			else if(windowMode==MODE_BTC_WINDOW){
				shotLogicBtc();
				gameWinLogicBtc();
			}

            isBreakDone=1;
            if(restartTimeFlag){
				timer.start();
			}
            if(windowMode==MODE_8BALL_WINDOW)newShotLogic8ball();
			else if(windowMode==MODE_9BALL_WINDOW)newShotLogic9ball();
			else if(windowMode==MODE_REPLICATION_WINDOW)newShotLogicReplication();
			else if(windowMode==MODE_BTC_WINDOW)newShotLogicBtc();
        }
        if(restartTimeFlag&&timer.getTicks()/1000>30){
            isFoul=1;
            curPlayer ^= 1;
            timer.start();
        }


    }
}

void declareWinner(){
    if(!isGameOver)return;
    SDL_Color textColor= {255,255,255};
    char text[20],won[]=" Won!!!";
    strAssign(text,userHandle[gameWinner]);
    strConcat(text,won);

    gTextWinTexture.loadFromRenderedText(text,textColor,gFontResult);

    int totWidth=gTextWinTexture.getWidth();
    int totHeight=gTextWinTexture.getHeight();

	gTextWinTexture.render((SCREEN_WIDTH-totWidth)/2,(SCREEN_HEIGHT-totHeight)/2);
}
