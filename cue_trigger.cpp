/* This module controls the triggering of cue for a shot */

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "include/consts.h"
#include "include/cue_trigger.h"
#include "include/sdl_handler.h"
#include <math.h>
#include "include/maths.h"
#include <stdio.h>

extern LTexture gCueTriggerTexture;
extern LTimer timer;
extern int isGameOver,isFoul,isTriggered;
extern double cueShift;

extern Mix_Chunk *gCueHitsBall;

CueSpeedTrigger::CueSpeedTrigger(int i1,int i2){
	height=gCueTriggerTexture.getHeight();
	width=gCueTriggerTexture.getWidth();
	x=i1;
	y=i2;
	inside=0;
	lim=(SCREEN_WIDTH-width)/2;
	initX=i1;
	isTriggered=0;
}

int CueSpeedTrigger::isInside(int x1,int y1){
	if(x1>initX+width||x1<initX||y1>y+height||y1<y)return 0;
	return 1;
}

void CueSpeedTrigger::handleEvent(SDL_Event& e,int &magnitude,int pauseTimerFlag){
	if(isTriggered||isGameOver||isFoul)return;
	if( e.type== SDL_MOUSEBUTTONDOWN){
		int x1,y1;
		SDL_GetMouseState(&x1,&y1);
		//printf("%d %d %d %d %d %d\n",x1,y1,x,y,height,width);
		//printf("%d %d %d %d\n",is_triggered,is_game_over,is_foul,inside);
		if(isInside(x1,y1)){
			//printf("%d %d %d %d\n",is_triggered,is_game_over,is_foul,inside);
			inside=1;
		}
	}
	else if(e.type==SDL_MOUSEBUTTONUP){
		int x1,y1;
		SDL_GetMouseState(&x1,&y1);
		if(inside && x1<=initX+width){
			Mix_PlayChannel( -1, gCueHitsBall, 0 );
			isTriggered=1;
			double tmp= MAX_VELOCITY*(double)((double)initX+(double)width-(double)x1)/(width);
			magnitude=tmp;
			//printf("DOUBLE MAGNITUDE: %lf\n%d\n%d\n%d\n",tmp,x1,init_x+width,width);
		   // printf("%d\n%d\n",x+width-x1,x+width);
			if(magnitude<1)magnitude=1;
			if(magnitude>(int)MAX_VELOCITY)magnitude=(int)MAX_VELOCITY;
			//printf("DOUBLE MAGNITUDE: %lf\n%d\n%d\n%d\n",magnitude,x1,init_x+width,width);
			if(pauseTimerFlag)timer.pause();
			cueShift=0;
		}
		inside=0;
		x=initX;
	}
	else if(e.type==SDL_MOUSEMOTION){
		if(inside){
			 int x1,y1;
			SDL_GetMouseState(&x1,&y1);
			if(x1>initX){
				//int diff= init_x+width
				x=min(initX,x1-width);
			   // y=max(20-height,y1-height);
			    cueShift= MAX_CUE_SHIFT*(initX+width-x1)/width;
			    if(cueShift<0)cueShift=0;
			}
		}
	}


}

void CueSpeedTrigger::render(){
	if(isTriggered)return;
	gCueTriggerTexture.render(x,y);
}
