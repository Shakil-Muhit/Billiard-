/* This module contains necassary mathematical functions used 
 * in the project */

#include <stdlib.h>
#include "include/maths.h"
#include <time.h>

void initSeed(){
	srand(time(0));
}

int getRandom(int a,int b){
    return a+rand()%(b-a+1);
}

double distanceSquared( int x1, int y1, int x2, int y2 )
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX*deltaX + deltaY*deltaY;
}

int max(int x,int y){
    if(x>y)return x;
    return y;
}

int min(int x,int y){
    if(x<y)return x;
    return y;
}
