/* This module controls the pipe and bigger balls and their
 * states and also the toggling status board in "Beat the Clock" mode */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "include/sdl_handler.h"
#include "include/consts.h"
#include "include/ball.h"
#include "include/modified_ball.h"

extern LTexture gModifiedBallsTexture[16],gBallPenaltyBoardTexture;
extern LTexture gModifiedLargePipeTexture,gTextBallPenaltyTexture;
extern Ball ball[16];
extern TTF_Font *gFontBallPenalty;

static int penaltyBoardFlag;

void ModifiedBall::init(int ind,int pt){
	r=gModifiedBallsTexture[ind].getWidth()/2;
	id=ind;
	points= pt;
}

void ModifiedBall::render(int x,int y){
	gModifiedBallsTexture[id].render(x-r,y-r);
	if(penaltyBoardFlag){
		int x2=x-gBallPenaltyBoardTexture.getWidth()/2;
		int y2=y-gBallPenaltyBoardTexture.getHeight()/2;
		gBallPenaltyBoardTexture.render(x2,y2);

		char revTxt[12],txt[12];
		int len=0,tmpPoints=points,negative=0;

		if(!tmpPoints)revTxt[len++]='0';
		if(tmpPoints<0){
			negative=1;
			tmpPoints= -tmpPoints;
		}

		while(tmpPoints){
			revTxt[len++]='0'+tmpPoints%10;
			tmpPoints/=10;
		}

		revTxt[len]='\0';
		if(negative)txt[0]='-';
		for(int i=len-1;i>=0;i--)txt[len-1-i+negative]=revTxt[i];
		txt[len+negative]='\0';



		if( gFontBallPenalty == NULL )
		{
			printf( "Failed to load ball penalty Italic font! SDL_ttf Error: %s\n", TTF_GetError() );
			return;
		}
		else {
			SDL_Color textColor = { 230, 0, 0 };
			if( !gTextBallPenaltyTexture.loadFromRenderedText( txt, textColor, gFontBallPenalty ) )
			{
				printf( "Failed to render ball penalty text texture!\n" );
				return;
			}
			int xTxt= x-gTextBallPenaltyTexture.getWidth()/2;
			int yTxt= y-gTextBallPenaltyTexture.getHeight()/2;
			gTextBallPenaltyTexture.render(xTxt,yTxt);
		}
	}
}

ModifiedBall modifiedBall[16];

void initModifiedBall(){
	penaltyBoardFlag=0;
	for(int ballId=1;ballId<16;ballId++)modifiedBall[ballId].init(ballId,ballId*BTC_BALL_POINTS_SCALE);
}

void handleEventModifiedBall(SDL_Event &e){
	if(e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_t){
		penaltyBoardFlag ^= 1;
	}
}

void renderModifiedPipe(){
	gModifiedLargePipeTexture.render(EXTRA_WIDTH-80,620);

	int x=EXTRA_WIDTH-70,y=620+gModifiedLargePipeTexture.getHeight()/2;

	for(int ballId=1;ballId<16;ballId++){
		if(ball[ballId].isInsidePocket)continue;
		int x1=x+gModifiedBallsTexture[ballId].getWidth()/2;
		modifiedBall[ballId].render(x1,y);
		x += gModifiedBallsTexture[ballId].getWidth()+8;
	}
}
