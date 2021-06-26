/* This module controls the "Leaderboards" section of the game and directs
 * to the leaderboards section of two single player modes */

#include <SDL2/SDL.h>
#include "include/sdl_handler.h"
#include "include/button.h"
#include "include/consts.h"

extern SDL_Renderer* gRenderer;
extern LTexture gButtonReplicationTexture,gButtonBtcTexture;
extern LTexture gButtonBackTexture,gLeaderboardBackgroundTexture;

static Button replicationButton,btcButton;
static Button backButton,returnButton;

void initAllLeaderboard(){
    replicationButton.init(444,273,gButtonReplicationTexture,LEADERBOARD_REPLICATION_WINDOW);
    btcButton.init(943,431,gButtonBtcTexture,LEADERBOARD_BTC_WINDOW);
    backButton.init(91,611,gButtonBackTexture,MAIN_MENU_WINDOW);
}

void handleEventLeaderboard(SDL_Event &e){
	replicationButton.handleButtonEvent(e);
	btcButton.handleButtonEvent(e);
	backButton.handleButtonEvent(e);
}

void renderAllLeaderboard(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );
	gLeaderboardBackgroundTexture.render(0,0);

	replicationButton.render(gButtonReplicationTexture,1);
	btcButton.render(gButtonBtcTexture,1);
	backButton.render(gButtonBackTexture,1);

	SDL_RenderPresent( gRenderer );
}
