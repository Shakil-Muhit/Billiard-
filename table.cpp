/* This module controls the state of the table */

#include "include/table.h"
#include "include/circle.h"
#include "include/consts.h"
#include "include/maths.h"
#include "include/ball.h"

Circle pocket[6];

void initPockets(){
	pocket[0].r=pocket[1].r=pocket[2].r=pocket[3].r=pocket[4].r=pocket[5].r=22;
	pocket[1].r=25;
	pocket[4].r=25;
	pocket[0].x=126+147;
	pocket[0].y=241;
	pocket[1].x=447+147;
	pocket[1].y=227;
	pocket[2].x=768+147;
	pocket[2].y=241;
	pocket[3].x=768+147;
	pocket[3].y=555;
	pocket[4].x=447+147;
	pocket[4].y=568;
	pocket[5].x=126+147;
	pocket[5].y=555;
}

int isBallPocketed(Ball ball){
	for(int i=0;i<6;i++){
		if((int)distanceSquared(pocket[i].x,pocket[i].y,ball.x,ball.y)<=pocket[i].r*pocket[i].r)return 1;
	}
	return 0;
}
