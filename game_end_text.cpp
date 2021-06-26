/*This module controls the rendering of text when result
 * happens in single player modes */

#include <SDL2/SDL.h>
#include "include/sdl_handler.h"
#include "include/ball.h"
#include "include/consts.h"
#include "include/string.h"

extern int isGameOver;
extern LTexture gTextWinMessageTexture;
extern TTF_Font *gFontResult;

void gameEndText(int isWon){
	if(!isGameOver)return;
	char txt[20]="You Lost!!!";
	if(isWon){
		char txt2[]="You Won!!!";
		strAssign(txt,txt2);
	}

	SDL_Color textColor = { 255, 255, 255 };
	gTextWinMessageTexture.loadFromRenderedText( txt, textColor, gFontResult );

	int x1=(SCREEN_WIDTH-gTextWinMessageTexture.getWidth())/2,y1=(SCREEN_HEIGHT-gTextWinMessageTexture.getHeight())/2;
	gTextWinMessageTexture.render(x1,y1);
}
