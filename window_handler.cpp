/* This module controls the current window of the game
 * and refers to expected modules and its functions */

#include <SDL2/SDL.h>
#include "include/window_handler.h"
#include "include/8_ball.h"
#include "include/9_ball.h"
#include "include/main_menu.h"
#include "include/play_menu.h"
#include "include/replication_mode.h"
#include "include/btc_mode.h"
#include "include/leaderboard.h"
#include "include/leaderboard_replication.h"
#include "include/leaderboard_btc.h"
#include "include/pause_menu.h"
#include "include/login_menu.h"
#include "include/help_menu.h"
#include "include/introduction_menu.h"
#include "include/controls_menu.h"
#include "include/rules_menu.h"
#include "include/rules_8ball.h"
#include "include/rules_9ball.h"
#include "include/rules_btc.h"
#include "include/rules_replication.h"
#include "include/string.h"
#include "include/consts.h"
#include <stdio.h>

int windowMode,prevWindowMode;
int userId[2],isLoggedIn;
char userHandle[2][20];
extern int quit;

void initAllWindowHandler(){
	windowMode=MAIN_MENU_WINDOW;
	prevWindowMode=0;
	userId[0]=userId[1]=DEFAULT_USER_ID;
	char guest1[]= "Player 1", guest2[]= "Player 2";
	strAssign(userHandle[0],guest1);
	strAssign(userHandle[1],guest2);
	isLoggedIn=0;
}

void handleEventWindowHandler(SDL_Event& e){
	if(windowMode==QUIT_GAME){
		quit= 1;
		return;
	}
	if(windowMode==MAIN_MENU_WINDOW){
		if(windowMode!=prevWindowMode){
			initAllMainMenu();
			prevWindowMode=windowMode;
		}
		handleEventMainMenu(e);
	}
	else if(windowMode==PLAY_MENU_WINDOW){
		if(windowMode!=prevWindowMode){
			initAllPlayMenu();
			prevWindowMode=windowMode;
		}
		handleEventPlayMenu(e);
	}
	else if(windowMode==MODE_9BALL_WINDOW){
		if(windowMode!=prevWindowMode){
			initAll9ball();
			prevWindowMode=windowMode;
		}
		handleEvent9ball(e);
	}
	else if(windowMode==MODE_REPLICATION_WINDOW){
		if(windowMode!=prevWindowMode){
			initAllReplication();
			prevWindowMode=windowMode;
		}
		handleEventReplication(e);
	}
	else if(windowMode==LEADERBOARD_WINDOW){
		if(windowMode!=prevWindowMode){
			initAllLeaderboard();
			prevWindowMode=windowMode;
		}
		handleEventLeaderboard(e);
	}
	else if(windowMode==LEADERBOARD_REPLICATION_WINDOW){
		if(windowMode!=prevWindowMode){
			initLeaderboardReplication();
			prevWindowMode=windowMode;
		}
		handleEventLeaderboardReplication(e);
	}
	else if(windowMode==MODE_BTC_WINDOW){
		if(windowMode!=prevWindowMode){
			initAllBtc();
			prevWindowMode=windowMode;
		}
		handleEventBtc(e);
	}
	else if(windowMode==LEADERBOARD_BTC_WINDOW){
		if(windowMode!=prevWindowMode){
			initLeaderboardBtc();
			prevWindowMode=windowMode;
		}
		handleEventLeaderboardBtc(e);
	}
	else if(windowMode==PAUSE_MENU_WINDOW){
		if(windowMode!=prevWindowMode){
			initAllPauseMenu();
			prevWindowMode=windowMode;
		}
		handleEventPauseMenu(e);
	}
	else if(windowMode==LOGIN_MENU_WINDOW){
		if(windowMode!=prevWindowMode){
			initAllLoginMenu();
			prevWindowMode=windowMode;
		}
		isLoggedIn=1;
		handleEventLoginMenu(e);
	}
	else if(windowMode==HELP_MENU_WINDOW){
		if(windowMode!=prevWindowMode){
			initAllHelpMenu();
			prevWindowMode=windowMode;
		}
		handleEventHelpMenu(e);
	}
	else if(windowMode==INTRODUCTION_MENU_WINDOW){
		if(windowMode!=prevWindowMode){
			initAllIntroductionMenu();
			prevWindowMode=windowMode;
		}
		handleEventIntroductionMenu(e);
	}
	else if(windowMode==CONTROLS_MENU_WINDOW){
		if(windowMode!=prevWindowMode){
			initAllControlsMenu();
			prevWindowMode=windowMode;
		}
		handleEventControlsMenu(e);
	}
	else if(windowMode==RULES_MENU_WINDOW){
		if(windowMode!=prevWindowMode){
			initAllRulesMenu();
			prevWindowMode=windowMode;
		}
		handleEventRulesMenu(e);
	}
	else if(windowMode==RULES_8BALL_MENU_WINDOW){
		if(windowMode!=prevWindowMode){
			initAllRules8ball();
			prevWindowMode=windowMode;
		}
		handleEventRules8ball(e);
	}
	else if(windowMode==RULES_9BALL_MENU_WINDOW){
		if(windowMode!=prevWindowMode){
			initAllRules9ball();
			prevWindowMode=windowMode;
		}
		handleEventRules9ball(e);
	}
	else if(windowMode==RULES_BTC_MENU_WINDOW){
		if(windowMode!=prevWindowMode){
			initAllRulesBtc();
			prevWindowMode=windowMode;
		}
		handleEventRulesBtc(e);
	}
	else if(windowMode==RULES_REPLICATION_MENU_WINDOW){
		if(windowMode!=prevWindowMode){
			initAllRulesReplication();
			prevWindowMode=windowMode;
		}
		handleEventRulesReplication(e);
	}
	else {
		if(windowMode!=prevWindowMode){
			initAll8ball();
			prevWindowMode=windowMode;
		}
		handleEvent8ball(e);
	}
}

void renderAllWindowHandler(){
	if(windowMode!=prevWindowMode)return;
	if(windowMode==MAIN_MENU_WINDOW){
		renderAllMainMenu();
	}
	else if(windowMode==PLAY_MENU_WINDOW){
		renderAllPlayMenu();
	}
	else if(windowMode==MODE_9BALL_WINDOW){
		renderAll9ball();
	}
	else if(windowMode==MODE_REPLICATION_WINDOW){
		renderAllReplication();
	}
	else if(windowMode==LEADERBOARD_WINDOW){
		renderAllLeaderboard();
	}
	else if(windowMode==LEADERBOARD_REPLICATION_WINDOW){
		renderLeaderboardReplication();
	}
	else if(windowMode==MODE_BTC_WINDOW){
		renderAllBtc();
	}
	else if(windowMode==LEADERBOARD_BTC_WINDOW){
		renderLeaderboardBtc();
	}
	else if(windowMode==PAUSE_MENU_WINDOW){
		renderAllPauseMenu();
	}
	else if(windowMode==LOGIN_MENU_WINDOW){
		renderAllLoginMenu();
	}
	else if(windowMode==HELP_MENU_WINDOW){
		renderAllHelpMenu();
	}
	else if(windowMode==INTRODUCTION_MENU_WINDOW){
		renderAllIntroductionMenu();
	}
	else if(windowMode==CONTROLS_MENU_WINDOW){
		renderAllControlsMenu();
	}
	else if(windowMode==RULES_MENU_WINDOW){
		renderAllRulesMenu();
	}
	else if(windowMode==RULES_8BALL_MENU_WINDOW){
		renderAllRules8ball();
	}
	else if(windowMode==RULES_9BALL_MENU_WINDOW){
		renderAllRules9ball();
	}
	else if(windowMode==RULES_BTC_MENU_WINDOW){
		renderAllRulesBtc();
	}
	else if(windowMode==RULES_REPLICATION_MENU_WINDOW){
		renderAllRulesReplication();
	}
	else {
		renderAll8ball();
	}
}
