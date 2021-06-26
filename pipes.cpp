/* This module controls the pipes which show the pocketed balls
 * in proper order */

#include "include/pipes.h"
#include "include/sdl_handler.h"

extern LTexture gSmallpipeTexture,gLargepipeTexture,gBallsTexture[16];
extern int ballsQueue[16],queueLen,ballType[16],playerType[2];

void ThreePipes::init(int x1,int y1,int x2,int y2,int x3,int y3){
	small1x=x1;
	small1y=y1;
	small2x=x2;
	small2y=y2;
	largex=x3;
	largey=y3;
	smallHeight=gSmallpipeTexture.getHeight();
	smallWidth=gSmallpipeTexture.getWidth();
	largeHeight=gLargepipeTexture.getHeight();
	largeWidth=gLargepipeTexture.getWidth();
}

void ThreePipes::render(){
	gSmallpipeTexture.render(small1x,small1y);
	gSmallpipeTexture.render(small2x,small2y);
	gLargepipeTexture.render(largex,largey);

	int nextX=largex+5;
	for(int i=0;i<queueLen;i++){
		int nextY=largey+largeHeight/2-gBallsTexture[ballsQueue[i]].getHeight()/2;
		gBallsTexture[ballsQueue[i]].render(nextX,nextY);
		nextX+= gBallsTexture[ballsQueue[i]].getWidth()+5;
	}

	if(playerType[0]!=-1){
		nextX=small1x+5;
		for(int i=0;i<queueLen;i++){
			if(ballType[ballsQueue[i]]!=playerType[0])continue;
			int nextY=small1y+smallHeight/2-gBallsTexture[ballsQueue[i]].getHeight()/2;
			gBallsTexture[ballsQueue[i]].render(nextX,nextY);
			nextX += gBallsTexture[ballsQueue[i]].getWidth()+5;
		}

		nextX=small2x+5;
		for(int i=0;i<queueLen;i++){
			if(ballType[ballsQueue[i]]!=playerType[1])continue;
			int nextY=small2y+smallHeight/2-gBallsTexture[ballsQueue[i]].getHeight()/2;
			gBallsTexture[ballsQueue[i]].render(nextX,nextY);
			nextX += gBallsTexture[ballsQueue[i]].getWidth()+5;
		}
	}

}
