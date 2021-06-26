/* This module controls the highlight with cue picture below the 
 * name of the current player in two player modes */

#include "include/sdl_handler.h"
#include "include/cue_hover.h"

extern LTexture gCueHoverTexture;
extern int curPlayer;

void CueHover::init(int i1,int j1,int i2,int j2){
	x1=i1;
	y1=j1;
	x2=i2;
	y2=j2;
}

void CueHover::render(){
	if(curPlayer)gCueHoverTexture.render(x2,y2);
	else gCueHoverTexture.render(x1,y1);
}
