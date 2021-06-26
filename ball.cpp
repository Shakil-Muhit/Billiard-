/* This module controls the state and events of all the balls.
 * Also contains many necessary info and variables of the overall game */

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL_mixer.h>
#include "include/ball.h"
#include "include/sdl_handler.h"
#include "include/consts.h"
#include "include/vec.h"
#include "include/maths.h"
#include "include/circle.h"
#include "include/table.h"
#include "include/window_handler.h"
#include "include/8_ball.h"
#include "include/9_ball.h"
#include "include/replication_mode.h"
#include "include/btc_mode.h"

int isTriggered=0;
char playerName[2][10]={"Player 1","Player 2"},winText[]=" Won!!!";
int isBreakDone=0,placeCue=0,firstTouchInBreak=0,firstTouch=0,curPlayer=0;
int anyBallScored=0,cueBallScored=0,ball9Scored=0;
int cushionFoul=1,foulInBreak=1,isFoul=1;
int ballType[16],playerType[]={-1,-1},ballTypesScored[]={0,0},totalScored[]={0,0};
int isGameOver=0,gameWinner=-1,gameLoser=-1;
int ballsQueue[16],queueLen=0;
int prevPlayerType[2];

extern Circle pocket[6];

extern Mix_Chunk *gBallPocketed;
extern Mix_Chunk *gBallHitsBallHard;
extern Mix_Chunk *gBallHitsBallSoft;

extern LTexture gBallsTexture[16];
extern LTexture gBoardTexture;
extern int windowMode;

void Ball::init(int x1,int y1,int ind){
	height=gBallsTexture[0].getHeight();
	width=gBallsTexture[0].getWidth();
	x=x1;
	y=y1;
	id=ind;
	isInsidePocket=0;
	magnitude=EPS;
	isMoving=0;
	velx=0;
	vely=0;
	r=width/2;
	inside=0;
}

int Ball::isInside(int x1,int y1,Ball ball){
	if(distanceSquared(x1,y1,ball.x,ball.y)>ball.r*ball.r*4.0)return 0;
	return 1;
}

int Ball::isVacant(int x1,int y1,Ball ball[]){
	int valid=1;
	double tmpX=x,tmpY=y;
	x=x1;
	y=y1;
	if(isBallPocketed(ball[id])){
		x=tmpX;
		y=tmpY;
		return 0;
	}

	x=tmpX;
	y=tmpY;

	for(int ballId=0;ballId<16;ballId++){
		if(ballId==id)continue;
		if(distanceSquared(x1,y1,ball[ballId].x,ball[ballId].y)<=ball[ballId].r*ball[ballId].r*4.0)valid=0;
	}
	if
	(x1-ball[0].r<=EXTRA_WIDTH+WALL_WIDTH+2||x1+ball[0].r>=SCREEN_WIDTH-WALL_WIDTH-EXTRA_WIDTH_END-4)valid=0;
	if(y1-ball[0].r<=WALL_WIDTH+EXTRA_HEIGHT||y1+ball[0].r>=SCREEN_HEIGHT-WALL_WIDTH-EXTRA_HEIGHT_END)valid=0;

	if(valid)return 1;
	return 0;
}

void Ball::placeBallRandomly(Ball &curBall,Ball ball[]){
	int ct=0;

	while(1){
		ct++;

		int st=EXTRA_WIDTH+WALL_WIDTH+curBall.r+2;
		if(foulInBreak)st=EXTRA_WIDTH+gBoardTexture.getWidth()*3.0/4.0;
		int x1=getRandom(st,SCREEN_WIDTH-WALL_WIDTH-curBall.r-2);
		int y1=getRandom(EXTRA_HEIGHT+WALL_WIDTH+curBall.r+2,SCREEN_HEIGHT-WALL_WIDTH-curBall.r-2);

		if(isVacant(x1,y1,ball)){
			curBall.init(x1,y1,id);
			break;
		}
	}

}

void Ball::cueBallDrag(Ball &cueBall,SDL_Event &e,Ball ball[]){
	if(!isFoul||isGameOver)return;
	if(foulInBreak&&x<EXTRA_WIDTH+gBoardTexture.getWidth()*3.0/4.0)placeBallRandomly(cueBall,ball);
	if( e.type== SDL_MOUSEBUTTONDOWN){
		int x1,y1;
		SDL_GetMouseState(&x1,&y1);
		if(isInside(x1,y1,ball[0])){
			inside=1;
		}
	}

	else if(e.type==SDL_MOUSEBUTTONUP){
		inside=0;
	}
	else if(e.type==SDL_MOUSEMOTION){
		if(inside){
			int x1,y1,valid=1;
			SDL_GetMouseState(&x1,&y1);

			if(isVacant(x1,y1,ball)){
				if(!foulInBreak||x1>=EXTRA_WIDTH+gBoardTexture.getWidth()*3.0/4.0)x=x1;
				y=y1;
			}
		}
	}

	else if(e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_RETURN){
		placeCue=1;
		isFoul=0;
		foulInBreak=0;
	}
}

void Ball::setVectorVelocity(Vec direction,double mag){
	if(isInsidePocket)return;
	magnitude=mag;
	if(magnitude<0||fabs(magnitude)<EPS){
		isMoving=0;
		velx=0;
		vely=0;
		return;
	}

	isMoving=1;
	Vec vel= {direction.x,direction.y};
	vel= scaleToMagnitude(vel,mag);
	velx= vel.x;
	vely= vel.y;
}

void Ball::recalcVel(){
	if(isInsidePocket || !isMoving)return;
	if(fabs(velx)<EPS){
		vely=magnitude;
		return;
	}

	Vec vel= {velx,vely};
	vel = scaleToMagnitude(vel,magnitude);
	velx= vel.x;
	vely= vel.y;
}

void Ball::handle2DCollision(Ball &ball1,Ball &ball2,Ball ball[]){
	point dball={ball2.x-ball1.x,ball2.y-ball1.y};
	double dist = sqrt(dball.x*dball.x+dball.y*dball.y);

	if(dist < ball1.r + ball2.r){
		if(ball1.magnitude>8)Mix_PlayChannel( -1, gBallHitsBallHard, 0 );
		else Mix_PlayChannel( -1, gBallHitsBallSoft, 0 );

		Ball b1=ball1,b2=ball2;
		double angle = atan2(dball.y,dball.x);
		double dx=dball.x,dy=dball.y;

		double x1 = 0, y1 = 0;
		double x2 = dx*cos(angle)+dy*sin(angle);
		double y2 = dy*cos(angle)-dx*sin(angle);

		double vx1 = ball1.velx*cos(angle)+ball1.vely*sin(angle);
		double vy1 = ball1.vely*cos(angle)-ball1.velx*sin(angle);
		double vx2 = ball2.velx*cos(angle)+ball2.vely*sin(angle);
		double vy2 = ball2.vely*cos(angle)-ball2.velx*sin(angle);

		double vx1final,vx2final;
		vx1final = vx2;
		vx2final = vx1;
		vx1 = vx1final;
		vx2 = vx2final;

	// fix the glitch by moving ball part equal to the overlap
	// see video for more details(https://youtu.be/guWIF87CmBg)

		double absSum = abs(vx1)+abs(vx2);
		double overlap = (ball1.r+ball2.r)-abs(x1-x2);
		if(absSum*overlap>0)x1 += vx1/absSum*overlap;
		if(absSum*overlap>0)x2 += vx2/absSum*overlap;

	// rotate the relative positions back
		double x1final = x1*cos(angle)-y1*sin(angle);
		double y1final = y1*cos(angle)+x1*sin(angle);
		double x2final = x2*cos(angle)-y2*sin(angle);
		double y2final = y2*cos(angle)+x2*sin(angle);


	 // finally compute the new absolute positions
		ball2.x = ball1.x + x2final;
		ball2.y = ball1.y + y2final;

		ball1.x = ball1.x + x1final;
		ball1.y = ball1.y + y1final;


		double curx=ball1.velx,cury=ball1.vely,cur2x=ball2.velx,cur2y=ball2.vely;
		//rotate vel back
		ball1.velx = vx1*cos(angle)-vy1*sin(angle);
		ball1.vely = vy1*cos(angle)+vx1*sin(angle);
		ball2.velx = vx2*cos(angle)-vy2*sin(angle);
		ball2.vely = vy2*cos(angle)+vx2*sin(angle);

		double v1=ball1.velx*ball1.velx+ball1.vely*ball1.vely;
		double v2=ball2.velx*ball2.velx+ball2.vely*ball2.vely;
		ball1.magnitude=sqrt(ball1.velx*ball1.velx+ball1.vely*ball1.vely);
		ball2.magnitude=sqrt(ball2.velx*ball2.velx+ball2.vely*ball2.vely);
		if(ball1.magnitude>0)ball1.isMoving=1;
		if(ball2.magnitude>0)ball2.isMoving=1;

		if(!firstTouchInBreak&&(ball1.id==0||ball2.id==0)){
			Vec dir={ball2.velx,ball2.vely};
			Ball cb=b1,otherBall=b2;
			if(ball2.id==0){
				dir={ball1.velx,ball2.vely};
				cb=b2;
				otherBall=b1;
			}
			if(otherBall.id!=11&&otherBall.id!=5&&otherBall.id!=4&&otherBall.id!=12){
				if(windowMode==MODE_8BALL_WINDOW)ball[8].setVectorVelocity({ball[0].velx,ball[0].vely},0.7*cb.magnitude);
				if(windowMode==MODE_8BALL_WINDOW)ball[14].setVectorVelocity({ball[0].velx,ball[0].vely},0.7*cb.magnitude);
			}
			firstTouchInBreak=otherBall.id;
		}

		if(!firstTouch){
			firstTouch=ball1.id;
			if(!firstTouch)firstTouch=ball2.id;
		}

	}
}

void Ball::move(Ball ball[]){
	if(!isTriggered || !isMoving || isInsidePocket)return;
	if(magnitude<1)magnitude -= 0.015;
	else magnitude -= 0.05;

	if(magnitude<0)magnitude=1e-10;

	if(fabs(magnitude)<EPS){
		magnitude=1e-9;
		velx=0;
		vely=0;
		isMoving=0;
		return;
	}
	recalcVel();


	double inc=1.0;

	for(double mg=0.01;;mg+=0.01){
		if(mg>1)break;
		point p={x+mg*velx,y+mg*vely};
		if(p.y-r<EXTRA_HEIGHT+WALL_WIDTH||p.y+r>SCREEN_HEIGHT-WALL_WIDTH-EXTRA_HEIGHT_END){
				inc=mg;
				break;
		}
		if(p.x-r<EXTRA_WIDTH+WALL_WIDTH+2||p.x+r>SCREEN_WIDTH-WALL_WIDTH-EXTRA_WIDTH_END-4){
			inc=mg;
			break;
		}
		int done=0;

		for(int ballId=0;ballId<16;ballId++){
			if(ballId==id)continue;
			if(dist(p,{ball[ballId].x,ball[ballId].y})<r+r){
				inc=mg;
				done=1;
				break;
			}
		}
		if(done)break;
	}

	x += inc*velx;
	y += inc*vely;


	if(isBallPocketed(ball[id]))isInsidePocket=1;

	if(isInsidePocket){
		Mix_PlayChannel( -1, gBallPocketed, 0 );
		velx=0;
		vely=0;
		x=-10;
		y=-10;
		magnitude=0;
		isMoving=0;
		cushionFoul=0;

		if(windowMode==MODE_8BALL_WINDOW)ballScoreLogic8ball(id);
		else if(windowMode==MODE_9BALL_WINDOW)ballScoreLogic9ball(id);
		else if(windowMode==MODE_REPLICATION_WINDOW)ballScoreLogicReplication(id);
		else if(windowMode==MODE_BTC_WINDOW)ballScoreLogicBtc(id);

		return;
	}

	if(y-r<EXTRA_HEIGHT+WALL_WIDTH||y+r>SCREEN_HEIGHT-WALL_WIDTH-EXTRA_HEIGHT_END){
		y -= inc*vely;y;
		vely= -vely;
		vely -= WALL_FRICTION*vely;
		velx -= WALL_FRICTION*velx;
		magnitude=sqrt(velx*velx+vely*vely);
		if(firstTouch)cushionFoul=0;
		initVely= -initVely;
	}

	if(x-r<EXTRA_WIDTH+WALL_WIDTH+2||x+r>SCREEN_WIDTH-WALL_WIDTH-EXTRA_WIDTH_END-4){
		x -= inc*velx;
		velx= -velx;
		vely -= WALL_FRICTION*vely;
		velx -= WALL_FRICTION*velx;
		magnitude=sqrt(velx*velx+vely*vely);
		if(firstTouch)cushionFoul=0;
		initVelx = -initVelx;
	}

	for(int ballId=0;ballId<16;ballId++){
		if(ballId!=id && !ball[ballId].isInsidePocket){
			handle2DCollision(ball[id],ball[ballId],ball);
		}
	}

}

void Ball::render(){
	if(isInsidePocket)return;
	gBallsTexture[id].render((int)x-r,(int)y-r);
}

Ball ball[16];
