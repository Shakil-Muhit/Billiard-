/* This module controls timer and contains some necessary 
 * functions about time */

#include "include/time.h"
#include "include/consts.h"
#include "include/maths.h"
#include "include/sdl_handler.h"
#include "include/string.h"
#include <stdio.h>

extern LTexture gTimerTexture,gTextTimeTexture[62];
extern int isGameOver;
extern LTimer timer;
extern TTF_Font *gFont;

static LTexture gTextElapsedTimeTexture;

void zeroPadTime(char tm[]){
	char sample[]="00:00:00",res[8];
	int len=strLen(tm);
	if(len>7)return;
	int extra= strlen(sample)-len;

	for(int i=0;i<extra;i++)res[i]=sample[i];
	for(int i=0;i<len;i++)res[i+extra]=tm[i];
	res[extra+len]='\0';
	strAssign(tm,res);
}

void formatTime(char tm[],int milliSecs){
	int secs=milliSecs/1000;
	int minutes=secs/60;
	int hours= minutes/60;
	secs %= 60;
	minutes %= 60;
	char revTime[20];
	int len=0,revLen=0,isHourOccured=0,isMinuteOccured=0;

	while(hours){
		isHourOccured=1;
		revTime[revLen++]='0'+hours%10;
		hours/=10;
	}

	for(int id=revLen-1;id>=0;id--)tm[len++]=revTime[id];

	revLen=0;

	while(minutes){
		isMinuteOccured=1;
		revTime[revLen++]='0'+minutes%10;
		minutes/=10;
	}

	if(isHourOccured){
		tm[len++]=':';
		if(revLen<2){
			tm[len++]='0';
		}
		if(revLen==0){
			tm[len++]='0';
		}
	}
	for(int id=revLen-1;id>=0;id--)tm[len++]=revTime[id];

	revLen=0;

	while(secs){
		revTime[revLen++]='0'+secs%10;
		secs/=10;
	}

	if(isHourOccured||isMinuteOccured){
		tm[len++]=':';
		if(revLen<2){
			tm[len++]='0';
		}
		if(revLen==0){
			tm[len++]='0';
		}
	}
	for(int id=revLen-1;id>=0;id--)tm[len++]=revTime[id];
	tm[len++]='\0';
}

void loadAndRenderElapsedTime(){
	if(isGameOver)timer.pause();
	char curTime[20];
	int milliSecs=timer.getTicks();

	formatTime(curTime,milliSecs);

	char tmp[20];
	strAssign(tmp,curTime);
	zeroPadTime(tmp);

	if(milliSecs<1000){
		curTime[0]='0';
		curTime[1]='\0';
	}

	SDL_Color textColor = { 230, 0, 0 };
	gTextElapsedTimeTexture.loadFromRenderedText( curTime, textColor,gFont );
	gTextElapsedTimeTexture.render(147+367+(gTimerTexture.getWidth()-gTextElapsedTimeTexture.getWidth())/2,72+(gTimerTexture.getHeight()-gTextElapsedTimeTexture.getHeight())/2);
}

void loadAndRenderRemainingTime(int totalTime){
	if(isGameOver)timer.pause();
	char curTime[20];
	int milliSecs=totalTime-timer.getTicks();
	milliSecs=max(0,milliSecs);

	toString(milliSecs/1000,curTime);

	if(milliSecs<1000){
		curTime[0]='0';
		curTime[1]='\0';
	}

	SDL_Color textColor = { 230, 0, 0 };
	gTextElapsedTimeTexture.loadFromRenderedText( curTime, textColor, gFont );
	gTextElapsedTimeTexture.render(147+367+(gTimerTexture.getWidth()-gTextElapsedTimeTexture.getWidth())/2,72+(gTimerTexture.getHeight()-gTextElapsedTimeTexture.getHeight())/2);
}

void renderTime(){
    if(isGameOver)timer.pause();
    int curTime= timer.getTicks();
    curTime/=1000;
    curTime=min(MAX_TIME,curTime);

    gTextTimeTexture[MAX_TIME-curTime].render(147+367+(gTimerTexture.getWidth()-gTextTimeTexture[MAX_TIME-curTime].getWidth())/2,72+(gTimerTexture.getHeight()-gTextTimeTexture[MAX_TIME-curTime].getHeight())/2);
}
