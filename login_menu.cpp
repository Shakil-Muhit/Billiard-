/* This module controls the login,register and change sides system 
 * of the game */

#include <SDL2/SDL.h>
#include "include/sdl_handler.h"
#include "include/consts.h"
#include "include/string.h"
#include "include/button.h"
#include "include/text_box.h"
#include "include/user.h"
#include <stdio.h>

extern SDL_Renderer* gRenderer;
extern TTF_Font *gFontTextInput;

extern LTexture gLoginBackgroundTexture;
extern LTexture gTextBoxTexture,gRectButtonLoginTexture[2];
extern LTexture gRectButtonCreateTexture[2],gRectButtonEntertexture[2];
extern LTexture gRectButtonLogoutTexture[2],gRectButtonChangeSidesTexture[2];
extern LTexture gRectButtonBackTexture[2],gRectButtonPlayAsGuestTexture[2];
extern LTexture gRectButtonRegisterTexture[2],gRectButtonProceedTexture[2];
extern LTexture gButtonPlayer1BallTexture,gButtonPlayer2BallTexture;
extern LTexture gNameInputTexture;
extern LTexture gTextPlayerTexture[2],gTextFailureTexture[TOTAL_FAILURE_TEXTS];
extern LTexture gTextReadyTexture;

extern int userId[2],totalUsers,windowMode,prevWindowMode;
extern char userHandle[2][20];
extern User userData[USER_LIMIT];

static const int INITIAL_STATE=0;
static const int LOGIN_STATE=1;
static const int REGISTER_STATE=2;
static const int READY_STATE=3;
static const int MAX_TIMER=300;

static Button player1BallButton,player2BallButton;
static RectButton loginButton[2],registerButton[2],backButton[2],createButton[2],enterButton[2];
static RectButton logoutButton[2],changeSidesButton,playAsGuestButton[2],proceedButton;
//static Rect_button login_button2,register_button2,back_button2,create_button2,enter_button2;
//static Rect_button logout_button2,play_as_guest_button2;

static Textbox usernameInput[2],passwordInput[2];
static char guest1[]="Player 1",guest2[]= "Player 2";
static char defaultTextUser[]= "Username", defaultTextPass[]= "Password";

static int userState[2], userToggle, targetWindow, userLogResponse[2];
static int textTimer[2];

void initAllLoginMenu(){
	int midX= 505+ gRectButtonPlayAsGuestTexture[0].getWidth()/2;
	player1BallButton.init(238,234,gButtonPlayer1BallTexture,-999);
	player2BallButton.init(970,544,gButtonPlayer2BallTexture,-999);
	loginButton[0].init(midX-gRectButtonLoginTexture[0].getWidth()/2,220,gRectButtonLoginTexture);
	registerButton[0].init(midX-gRectButtonRegisterTexture[0].getWidth()/2,270,gRectButtonRegisterTexture);
	loginButton[1].init(midX-gRectButtonLoginTexture[0].getWidth()/2,525,gRectButtonLoginTexture);
	registerButton[1].init(midX-gRectButtonRegisterTexture[0].getWidth()/2,570,gRectButtonRegisterTexture);
	backButton[0].init(midX-gRectButtonBackTexture[0].getWidth()/2,265,gRectButtonBackTexture);
	backButton[1].init(midX-gRectButtonBackTexture[0].getWidth()/2,570,gRectButtonBackTexture);
	createButton[0].init(midX-gRectButtonCreateTexture[0].getWidth()/2,220,gRectButtonCreateTexture);
	createButton[1].init(midX-gRectButtonCreateTexture[0].getWidth()/2,525,gRectButtonCreateTexture);
	logoutButton[0].init(midX-gRectButtonLogoutTexture[0].getWidth()/2,265,gRectButtonLogoutTexture);
	logoutButton[1].init(midX-gRectButtonLogoutTexture[0].getWidth()/2,570,gRectButtonLogoutTexture);
	enterButton[0].init(midX-gRectButtonEntertexture[0].getWidth()/2,220,gRectButtonEntertexture);
	enterButton[1].init(midX-gRectButtonEntertexture[0].getWidth()/2,525,gRectButtonEntertexture);
	playAsGuestButton[0].init(505,170,gRectButtonPlayAsGuestTexture);
	playAsGuestButton[1].init(505,475,gRectButtonPlayAsGuestTexture);
	proceedButton.init(994,653,gRectButtonProceedTexture);
	changeSidesButton.init(midX-gRectButtonChangeSidesTexture[0].getWidth()/2,371,gRectButtonChangeSidesTexture);

	usernameInput[0].init(98,340,gTextBoxTexture.getWidth(),gTextBoxTexture.getHeight(),defaultTextUser);
	passwordInput[0].init(98,400,gTextBoxTexture.getWidth(),gTextBoxTexture.getHeight(),defaultTextPass);
	usernameInput[1].init(830,340,gTextBoxTexture.getWidth(),gTextBoxTexture.getHeight(),defaultTextUser);
	passwordInput[1].init(830,400,gTextBoxTexture.getWidth(),gTextBoxTexture.getHeight(),defaultTextPass);

	initUser();
	if(prevWindowMode==MAIN_MENU_WINDOW){
		userState[0]=userState[1]=INITIAL_STATE;
		userToggle=0;
		targetWindow=PLAY_MENU_WINDOW;
	}
	else targetWindow=prevWindowMode;

	userLogResponse[0]=userLogResponse[1]=-1;
	textTimer[0]=textTimer[1]=0;
}

void handleEventLoginMenu(SDL_Event& e){
	//textbox.handle_event(e);
//	textbox2.handle_event(e);
	for(int i=0;i<2;i++){
		int curId= i^userToggle;
		if(userState[curId]==INITIAL_STATE){
			if(e.type==SDL_MOUSEBUTTONDOWN){
				if(playAsGuestButton[i].isMouseInside()){
					userState[curId]=READY_STATE;
					userId[curId]=DEFAULT_USER_ID;
				}
				else if(loginButton[i].isMouseInside()){
					userState[curId]=LOGIN_STATE;
				}
				else if(registerButton[i].isMouseInside()){
					userState[curId]=REGISTER_STATE;
				}
			}
		}
		else if(userState[curId]==LOGIN_STATE){

			usernameInput[curId].handleEvent(e);
			passwordInput[curId].handleEvent(e);

			if(e.type==SDL_MOUSEBUTTONDOWN){
				if(backButton[i].isMouseInside()){
					userState[curId]=INITIAL_STATE;
					strClear(usernameInput[curId].inputText);
					strClear(passwordInput[curId].inputText);
					usernameInput[curId].textLen=0;
					passwordInput[curId].textLen=0;
					textTimer[curId]=0;
				}
				else if(enterButton[i].isMouseInside()){
					if(strCompare(usernameInput[curId].inputText,userHandle[curId^1])!=0){
						int response= authenticate(usernameInput[curId].inputText,passwordInput[curId].inputText,userId[curId]);
						printf("RES %d\n",response);
						if(response==-1){
							userLogResponse[curId]=F_HANDLE_NOT_FOUND;
							textTimer[curId]=MAX_TIMER;
						}
						else if(!response){
							userLogResponse[curId]=F_PASSWORD;
							textTimer[curId]=MAX_TIMER;
						}

						if(response==1){
							userState[curId]=READY_STATE;
							strAssign(userHandle[curId],userData[userId[curId]].name);
							strClear(usernameInput[curId].inputText);
							strClear(passwordInput[curId].inputText);
							usernameInput[curId].textLen=0;
							passwordInput[curId].textLen=0;
							textTimer[curId]=0;
						}
					}
					else {
						userLogResponse[curId]= F_COLLIDE;
						textTimer[curId]=MAX_TIMER;
					}
				}
			}
		}
		else if(userState[curId]==REGISTER_STATE){
			usernameInput[curId].handleEvent(e);
			passwordInput[curId].handleEvent(e);

			if(e.type==SDL_MOUSEBUTTONDOWN){
				if(backButton[i].isMouseInside()){
					userState[curId]=INITIAL_STATE;
					strClear(usernameInput[curId].inputText);
					strClear(passwordInput[curId].inputText);
					usernameInput[curId].textLen=0;
					passwordInput[curId].textLen=0;
					textTimer[curId]=0;
				}
				else if(createButton[i].isMouseInside()){
					int response= addUser(usernameInput[curId].inputText,passwordInput[curId].inputText);
					printf("RES %d\n",response);
					if(response==1){
						userState[curId]=READY_STATE;
						userId[curId]=totalUsers-1;
						strAssign(userHandle[curId],usernameInput[curId].inputText);
						strClear(usernameInput[curId].inputText);
						strClear(passwordInput[curId].inputText);
						usernameInput[curId].textLen=0;
						passwordInput[curId].textLen=0;
						textTimer[curId]=0;
					}
					else if(response==-1){
						userLogResponse[curId]=F_EMPTY;
						textTimer[curId]=MAX_TIMER;
					}
					else {
						userLogResponse[curId]=F_HANDLE_EXISTS;
						textTimer[curId]=MAX_TIMER;
					}
				}
			}
		}

		else if(userState[curId]==READY_STATE){
			if(userId[curId]==DEFAULT_USER_ID){
				if(e.type==SDL_MOUSEBUTTONDOWN){
					if(backButton[i].isMouseInside()){
						userState[curId]= INITIAL_STATE;
						if(i)strAssign(userHandle[i],guest2);
						else strAssign(userHandle[i],guest1);
					}
				}
			}
			else {
				if(e.type==SDL_MOUSEBUTTONDOWN){
					if(logoutButton[i].isMouseInside()){
						userState[curId]= INITIAL_STATE;
						userId[curId]= DEFAULT_USER_ID;
						if(i)strAssign(userHandle[i],guest2);
						else strAssign(userHandle[i],guest1);
					}
				}
			}
		}
	}

	if(e.type==SDL_MOUSEBUTTONDOWN && changeSidesButton.isMouseInside()){
		for(int i=0;i<2;i++){
			strClear(usernameInput[i].inputText);
			strClear(passwordInput[i].inputText);
			usernameInput[i].textLen=0;
			passwordInput[i].textLen=0;
		}
		strSwap(userHandle[0],userHandle[1]);

		int tmp= userId[0];
		userId[0]= userId[1];
		userId[1]= tmp;

		tmp= userState[0];
		userState[0]= userState[1];
		userState[1]= tmp;

		userToggle ^= 1;
	}

	if(e.type==SDL_MOUSEBUTTONDOWN && userState[0]==READY_STATE && userState[1]==READY_STATE){
		if(proceedButton.isMouseInside()){
			SDL_Color textColor = { 0, 0, 0 };
			gTextPlayerTexture[0].loadFromRenderedText( userHandle[0], textColor, gFontTextInput );
			gTextPlayerTexture[1].loadFromRenderedText( userHandle[1], textColor, gFontTextInput );
			windowMode=targetWindow;
		}
	}
}

void renderAllLoginMenu(){
	SDL_Color textColor = { 0xFF, 0xFF, 0xFF, 0xFF };
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );
	//printf("%d\n",user1_state);

	gLoginBackgroundTexture.render(0,0);


	//login_button1.render(gRectButtonLoginTexture,1);
	//login_button2.render(gRectButtonLoginTexture,1);
	//register_button1.render(gRectButtonRegisterTexture,1);
	//register_button2.render(gRectButtonRegisterTexture,1);
	//back_button.render(gRectButtonBackTexture,1);

	changeSidesButton.render(gRectButtonChangeSidesTexture,1);
	int textBoxX[2]= {98,830};

	for(int i=0;i<2;i++){
		int curId=i^userToggle;
		//if(i)text_box_x= 900;
		if(userState[curId]==INITIAL_STATE){
			playAsGuestButton[i].render(gRectButtonPlayAsGuestTexture,1);
			loginButton[i].render(gRectButtonLoginTexture,1);
			registerButton[i].render(gRectButtonRegisterTexture,1);
		}
		else if(userState[curId]==LOGIN_STATE){
			enterButton[i].render(gRectButtonEntertexture,1);
			backButton[i].render(gRectButtonBackTexture,1);
			gTextBoxTexture.render(textBoxX[curId],340);
			gTextBoxTexture.render(textBoxX[curId],400);
			usernameInput[curId].render(textColor,gFontTextInput,0);
			passwordInput[curId].render(textColor,gFontTextInput,1);
		}
		else if(userState[curId]==REGISTER_STATE){
			createButton[i].render(gRectButtonCreateTexture,1);
			backButton[i].render(gRectButtonBackTexture,1);
			gTextBoxTexture.render(textBoxX[curId],340);
			gTextBoxTexture.render(textBoxX[curId],400);
			usernameInput[curId].render(textColor,gFontTextInput,0);
			passwordInput[curId].render(textColor,gFontTextInput,1);
		}
		else if(userState[curId]==READY_STATE){
			if(userId[curId]==DEFAULT_USER_ID){
				backButton[i].render(gRectButtonBackTexture,1);
			}
			else {
				logoutButton[i].render(gRectButtonLogoutTexture,1);
			}
		}
	}
	SDL_Color textColor2 = { 0, 0, 0, 0xFF };

	if(!userToggle){
		gButtonPlayer1BallTexture.render(135,136);
		gButtonPlayer2BallTexture.render(865,441);

		gNameInputTexture.loadFromRenderedText(userHandle[0],textColor2,gFontTextInput);
		gNameInputTexture.render(241-gNameInputTexture.getWidth()/2,253-gNameInputTexture.getHeight()/2);
		if(userLogResponse[0]!=-1 && textTimer[0]){
			int width= gTextFailureTexture[userLogResponse[0]].getWidth();
			gTextFailureTexture[userLogResponse[0]].render(238-width/2,450);
		}
		if(userState[0]==READY_STATE){
			int width= gTextReadyTexture.getWidth();
			gTextReadyTexture.render(238-width/2,360);
		}

		gNameInputTexture.loadFromRenderedText(userHandle[1],textColor2,gFontTextInput);
		gNameInputTexture.render(960-gNameInputTexture.getWidth()/2,548-gNameInputTexture.getHeight()/2);
		if(userLogResponse[1]!=-1 && textTimer[1]){
			int width= gTextFailureTexture[userLogResponse[1]].getWidth();
			gTextFailureTexture[userLogResponse[1]].render(970-width/2,310);
		}

		if(userState[1]==READY_STATE){
			int width= gTextReadyTexture.getWidth();
			gTextReadyTexture.render(970-width/2,360);
		}
	}
	else {
		gButtonPlayer1BallTexture.render(865,136);
		gButtonPlayer2BallTexture.render(135,441);

		gNameInputTexture.loadFromRenderedText(userHandle[1],textColor2,gFontTextInput);
		gNameInputTexture.render(967-gNameInputTexture.getWidth()/2,253-gNameInputTexture.getHeight()/2);
		if(userLogResponse[1]!=-1 && textTimer[1]){
			int width= gTextFailureTexture[userLogResponse[1]].getWidth();
			gTextFailureTexture[userLogResponse[1]].render(970-width/2,450);
		}
		if(userState[0]==READY_STATE){
			int width= gTextReadyTexture.getWidth();
			gTextReadyTexture.render(238-width/2,360);
		}

		gNameInputTexture.loadFromRenderedText(userHandle[0],textColor2,gFontTextInput);
		gNameInputTexture.render(231-gNameInputTexture.getWidth()/2,548-gNameInputTexture.getHeight()/2);
		if(userLogResponse[0]!=-1 && textTimer[0]){
			int width= gTextFailureTexture[userLogResponse[0]].getWidth();
			gTextFailureTexture[userLogResponse[0]].render(238-width/2,310);
		}
		if(userState[1]==READY_STATE){
			int width= gTextReadyTexture.getWidth();
			gTextReadyTexture.render(970-width/2,360);
		}
	}
	if(userState[0]==READY_STATE && userState[1]==READY_STATE)proceedButton.render(gRectButtonProceedTexture,1);

	if(textTimer[0])textTimer[0]--;
	if(textTimer[1])textTimer[1]--;

	SDL_RenderPresent( gRenderer );
}
