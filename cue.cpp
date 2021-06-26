/* This module controls the state and events of the cue */

#include "include/cue.h"
#include <SDL2/SDL.h>
#include "include/ball.h"
#include "include/sdl_handler.h"
#include "include/vec.h"
#include "include/aimline.h"

extern LTexture gCueTexture;
extern int isFoul,isGameOver,isTriggered;
double cueShift;

void Cue::init(int i1,int i2,point c){
	height=gCueTexture.getHeight();
	width=gCueTexture.getWidth();
	x=i1;
	y=i2;
	inside=0;
	angle=0;
	origin=c;

	leftTop={(double)x,(double)y};
	leftBottom={(double)x,(double)y+height};
	rightTop={(double)x+width/2-10,(double)y};
	rightBottom={(double)x+width/2-10,(double)y+height};

	lb=tovec(origin,leftBottom);
	lt=tovec(origin,leftTop);
	rt=tovec(origin,rightTop);
	rb=tovec(origin,rightBottom);
	point rightMid={(rightBottom.x+rightTop.x)/2,(rightBottom.y+rightTop.y)/2};
	currentDirection=tovec(rightMid,origin);
	currentLb=leftBottom;
	cueShift=0;
}

void Cue::rotatePoints(double ang){
	Vec curLb=rotateVec(lb,ang);
	Vec curLt=rotateVec(lt,ang);
	Vec curRt=rotateVec(rt,ang);
	Vec curRb=rotateVec(rb,ang);

	leftBottom=translate(origin,curLb);
	leftTop=translate(origin,curLt);
	rightTop=translate(origin,curRt);
	rightBottom=translate(origin,curRb);
	point rightMid={(rightBottom.x+rightTop.x)/2,(rightBottom.y+rightTop.y)/2};

	currentDirection=tovec(rightMid,origin);
	currentLb=leftBottom;
}

int Cue::isInside(int x1,int y1){
	point p={(double)x1,(double)y1};
	point walls[4]={leftBottom,leftTop,rightTop,rightBottom};

	int cc= ccw(walls[0],walls[1],p);
	for(int i=1;i<4;i++){
		if(cc!=ccw(walls[i],walls[(i+1)%4],p))return 0;
	}

	return 1;
}

void Cue::handleEvent(SDL_Event& e){
	if(isTriggered||isGameOver)return;
	if( e.type== SDL_MOUSEBUTTONDOWN){
		int x1,y1;
		SDL_GetMouseState(&x1,&y1);
		if(isInside(x1,y1)){
			inside=1;
		}
	}
	else if(e.type==SDL_MOUSEBUTTONUP){
		inside=0;
	}
	else if(e.type==SDL_MOUSEMOTION){
		if(inside){
			int x1,y1;
			SDL_GetMouseState(&x1,&y1);
			point p1(x,y),p0(x+width/2,y+height/2),p2(x1,y1);
			double radAngle=getAngleClockwise(p1,p0,p2);
			angle= RadToDegree(radAngle);
			rotatePoints(radAngle);
		}
	}
}

void Cue::render(Ball ball[]){
	if(isTriggered||isFoul||isGameOver)return;
	Vec shift= {currentDirection.x,currentDirection.y};
	shift=scaleToMagnitude(shift,cueShift);
	gCueTexture.render(x-shift.x,y-shift.y,NULL,angle);
	renderAimline(ball,currentDirection,origin,rightBottom);
}
