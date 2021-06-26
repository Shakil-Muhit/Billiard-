/* This module controls the textbox for prompting user input
 * for names and password in login menu */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "include/sdl_handler.h"
#include "include/text_box.h"
#include "include/string.h"
#include "include/consts.h"
#include <stdio.h>

extern LTexture gTextBoxTexture, gInputTextTexture, gSmallpipeTexture;

int Textbox::isMouseInside(){
	int x1,y1;
	SDL_GetMouseState(&x1,&y1);
	if(x1<x||x1>x+width||y1<y||y1>y+height)return 0;
	return 1;
}

void Textbox::init(int x1,int y1,int w,int h,char def[]){
	x=x1;
	y=y1;
	//height=tex.getHeight();
	//width=tex.getWidth();
	height=h;
	width= w;
	//printf("%d %d\n",height,width);
	renderText=1;
	while(textLen){
		textLen--;
		inputText[textLen]='\0';
	}
	gInputTextTexture.free();
	onFocus= 0;
	textLen= 0;
	strAssign(defaultText,def);
	frame=0;
}

void Textbox::handleEvent(SDL_Event& e){
	if(e.type== SDL_MOUSEBUTTONDOWN)onFocus = isMouseInside();

	if(!onFocus){
		//SDL_StopTextInput();
		return;
	}
	else {
		SDL_StartTextInput();
	}
	renderText=0;

	if( e.type == SDL_KEYDOWN )
	{

		//Handle backspace
		if( e.key.keysym.sym == SDLK_BACKSPACE && textLen > 0 )
		{
			//lop off character
			inputText[textLen-1]='\0';
			textLen--;
			renderText = 1;
		}
		//Handle copy
		else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
		{
			SDL_SetClipboardText( inputText );
		}
		////Handle paste
		//else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
		//{
			//inputText = SDL_GetClipboardText();
			//text_len=0;
			//while(inputText[text_len])text_len++;
			//renderText = 1;
		//}
		else if( e.key.keysym.sym == SDLK_RETURN){
			//Disable text input
            //SDL_StopTextInput();
            onFocus= 0;
        }
	}
	//Special text input event
	else if( e.type == SDL_TEXTINPUT )
	{
		//Not copy or pasting
		if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) )
		{
			//Append character
			int len2=0;
			while(textLen < TEXT_INPUT_LIMIT && e.text.text[len2]){
				inputText[textLen]=e.text.text[len2];
				textLen++;
				len2++;
			}
			renderText = 1;
		}
	}
}

void Textbox::render(SDL_Color textColor, TTF_Font *font, int isPassword){
	//Text is not empty
	if( textLen )
	{
		//Render new text
		//printf("YES %d %d\n",x,y);
		if(!isPassword)gInputTextTexture.loadFromRenderedText( inputText, textColor, font );
		else {
			char hiddenTxt[20];
			for(int i=0;i<textLen;i++)hiddenTxt[i]= '*';
			hiddenTxt[textLen]='\0';
			gInputTextTexture.loadFromRenderedText( hiddenTxt, textColor, font );
		}
	}
	//Text is empty
	else
	{
		//Render space texture
		char whitespace[]= " ";
		if(onFocus)gInputTextTexture.loadFromRenderedText( whitespace, textColor, font );
		else gInputTextTexture.loadFromRenderedText( defaultText, textColor, font);
	}
	gInputTextTexture.render(x+15,y+22-gInputTextTexture.getHeight()/2);
	if(onFocus && frame<30){
		char ch[]= "|";
		int ext = gInputTextTexture.getWidth();
		gInputTextTexture.loadFromRenderedText(ch,textColor,font);
		gInputTextTexture.render(x+10+ext,y);
	}
	frame++;
	frame %= 50;
}

