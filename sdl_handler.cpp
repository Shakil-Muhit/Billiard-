/* This module handles all the functions directly related to SDL.
 * Also provides abstraction of texture and timer functions */

#include "include/sdl_handler.h"
#include "include/consts.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string.h>
#include "include/ball.h"

extern char playerName[2][10],winText[];

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;
//Globally used font
TTF_Font *gFont = NULL,*gFontTimer=NULL,*gFontBallPenalty=NULL;
TTF_Font *gFontTextInput=NULL, *gFontFailure=NULL;
TTF_Font *gFontTips=NULL, *gFontResult=NULL, *gFontBtcEffect=NULL;

//The music that will be played
Mix_Music *gBackgroundMusic = NULL;

//The sound effects that will be used
Mix_Chunk *gCueHitsBall = NULL;
Mix_Chunk *gBallHitsBallHard = NULL;
Mix_Chunk *gBallHitsBallSoft = NULL;
Mix_Chunk *gClap = NULL;
Mix_Chunk *gGroan = NULL;
Mix_Chunk *gCrowdCheer = NULL;
Mix_Chunk *gBallPocketed = NULL;
Mix_Chunk *gCueHitsBallHover = NULL;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( char path[] )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( char textureText[], SDL_Color textColor, TTF_Font *font)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( font, textureText, textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}


	//Return success
	return mTexture != NULL;
}

#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LTexture gBoardTexture,gBallsTexture[16],gCueTexture,gCueTriggerTexture,gBackgroundTexture[3],gTimerTexture;
LTexture gModifiedBallsTexture[16];
LTexture gTextPlayerTexture[2],gTextWinTexture,gTextTimeTexture[62];
LTexture gCueHoverTexture;
LTexture gLargepipeTexture,gSmallpipeTexture;
LTexture gMenu1BackgroundTexture,gPlayMenuBackgroundTexture;
LTexture gPauseBackgroundTexture,gLoginBackgroundTexture;
LTexture gButtonPlayTexture,gButtonExitTexture,gButtonHelpTexture,gButtonLeaderboardTexture;
LTexture gButton8BallTexture,gButton9BallTexture,gButtonReplicationTexture;
LTexture gButtonReturnTexture,gButtonBackTexture,gButtonBtcTexture;
LTexture gButtonReturnGameTexture,gButtonRestartTexture,gButtonQuitTexture;
LTexture gButtonMenuTexture,gButtonChangeSidesTexture;
LTexture gButtonPlayer1BallTexture,gButtonPlayer2BallTexture;
LTexture gButtonIntroductionTexture,gButtonRulesTexture,gButtonControlsTexture;
LTexture gButtonNextTexture;
LTexture gNameInputTexture;
LTexture gLeaderboardReplicationTexture,gLeaderboardBtcTexture;
LTexture gLeaderboardBackgroundTexture;
LTexture gRecordNameTexture[LEADERBOARD_SIZE];
LTexture gRecordDurationTexture[LEADERBOARD_SIZE];
LTexture gModifiedLargePipeTexture,gBallPenaltyBoardTexture;
LTexture gTextBoxTexture,gRectButtonLoginTexture[2],gRectButtonRegisterTexture[2];
LTexture gRectButtonCreateTexture[2],gRectButtonEntertexture[2],gRectButtonProceedTexture[2];
LTexture gRectButtonLogoutTexture[2],gRectButtonChangeSidesTexture[2];
LTexture gRectButtonBackTexture[2],gRectButtonPlayAsGuestTexture[2];
LTexture gInputTextTexture;
LTexture gIntroductionBgTexture[3],gControlsBgTexture[2],gRules8ballBgTexture[3];
LTexture gRules9ballBgTexture[3],gRulesBtcBgTexture[3],gRulesReplicationBgTexture[2];
LTexture gTextWinMessageTexture,gTextBallPenaltyTexture;
LTexture gTextFailureTexture[TOTAL_FAILURE_TEXTS];
LTexture gTextTipsTexture[TOTAL_TIPS];
LTexture gTextReadyTexture,gTextCongratsTexture;
LTexture gTextRewardtexture,gTextPenaltytexture;


LTimer::LTimer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

void LTimer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
    //The actual timer time
    Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool LTimer::isStarted()
{
    //Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
    //Timer is running and paused
    return mPaused && mStarted;
}

LTimer timer;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Billiard++", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

                 //Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
			}
		}
	}

	//Initialize SDL_mixer
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	char fontPath[]= "fonts/Lato-Italic.ttf";
    gFont = TTF_OpenFont( fontPath, 32 );
    if( gFont == NULL )
    {
        printf( "Failed to load Italic font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        SDL_Color textColor = { 230, 0, 0 };
        for(int sec=0;sec<=60;sec++){
            char curSec[5];
            int rem= sec%10,div=sec/10;

            if(div){
                curSec[0]='0'+div;
                curSec[1]='0'+rem;
            }
            else {
                curSec[0]='0'+ rem;
                curSec[1]='\0';
            }
            curSec[2]='\0';

            if( !gTextTimeTexture[sec].loadFromRenderedText( curSec, textColor,gFont ) )
            {
                printf( "Failed to render Player 1 text texture!\n" );
                success = false;
            }
        }
    }

    char fontGothicPath[]= "fonts/CenturyGothic.ttf";
    gFontTextInput = TTF_OpenFont( fontGothicPath, 28 );

    if( gFontTextInput == NULL )
    {
        printf( "Failed to load text input font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    char fontGothicBoldPath[]= "fonts/CenturyGothicBold.ttf";
    gFontResult = TTF_OpenFont( fontGothicBoldPath, 100 );

    if( gFontResult == NULL )
    {
        printf( "Failed to load result font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    gFontBtcEffect = TTF_OpenFont( fontGothicPath, 40 );

    if( gFontBtcEffect == NULL )
    {
        printf( "Failed to load BTC Ball effect font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else {
		SDL_Color textColor={255,0,0};
		char penaltyText[]=" -10";
		gTextPenaltytexture.loadFromRenderedText(penaltyText,textColor,gFontBtcEffect);

		textColor={0,0xcc,0};
		char rewardText[]=" +10";
		gTextRewardtexture.loadFromRenderedText(rewardText,textColor,gFontBtcEffect);
	}

    gFontFailure = TTF_OpenFont( fontGothicPath, 14 );

    if( gFontFailure == NULL )
    {
        printf( "Failed to load failure text font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else {
		SDL_Color textColor= {255,0,0};
		char handle404[]= "USERNAME NOT FOUND!";
		gTextFailureTexture[F_HANDLE_NOT_FOUND].loadFromRenderedText( handle404,textColor,gFontFailure);

		char handleExists[]= "USERNAME ALREADY EXISTS!";
		gTextFailureTexture[F_HANDLE_EXISTS].loadFromRenderedText( handleExists,textColor,gFontFailure);

		char pass404[]= "PASSWORD DOES NOT MATCH!";
		gTextFailureTexture[F_PASSWORD].loadFromRenderedText( pass404,textColor,gFontFailure);

		char collides[]= "OTHER USER IS LOGGED IN TO THIS ID!";
		gTextFailureTexture[F_COLLIDE].loadFromRenderedText( collides,textColor,gFontFailure);

		char empty[]= "USERNAME OR PASSWORD CAN'T BE EMPTY!";
		gTextFailureTexture[F_EMPTY].loadFromRenderedText(empty,textColor,gFontFailure);
	}

	gFontTips = TTF_OpenFont( fontGothicPath, 16 );

	if( gFontTips == NULL )
    {
        printf( "Failed to load tips text font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else {
		SDL_Color textColor= {0,0,0};
		char drag[]= "USE THE MOUSE TO DRAG AND PRESS ENTER TO PLACE THE CUE BALL";
		gTextTipsTexture[T_DRAG].loadFromRenderedText(drag,textColor,gFontTips);

		char toggle[]= "PRESS 'T' TO TOGGLE THE STATUS BOARD OF THE BALLS";
		gTextTipsTexture[T_TOGGLE].loadFromRenderedText(toggle,textColor,gFontTips);
	}

	char fontPenaltyPath[]= "fonts/Lato-Italic.ttf";

    gFontBallPenalty = TTF_OpenFont( fontPenaltyPath, 14 );

    if( gFontBallPenalty == NULL )
    {
        printf( "Failed to load Italic font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    char readyPath[]= "images/ready_text.png";

    if(!gTextReadyTexture.loadFromFile( readyPath )){
		printf( "Failed to load ready text texture!\n" );
		success=false;
	}

	char congratsPath[]= "images/congrats_text.png";

    if(!gTextCongratsTexture.loadFromFile( congratsPath )){
		printf( "Failed to load congrats text texture!\n" );
		success=false;
	}

	char mainMenuPath[]= "images/menu_background1.png";
    if( !gMenu1BackgroundTexture.loadFromFile( mainMenuPath ) )
	{
		printf( "Failed to load main menu texture!\n" );
		success = false;
	}

	char playMenuPath[]= "images/play_menu _background.png";
	if( !gPlayMenuBackgroundTexture.loadFromFile( playMenuPath ) )
	{
		printf( "Failed to load play menu texture!\n" );
		success = false;
	}

	char leaderboardReplicationPath[]= "images/leaderboard_background_replication.png";
	if( !gLeaderboardReplicationTexture.loadFromFile( leaderboardReplicationPath ) )
	{
		printf( "Failed to load leaderboard background replication texture!\n" );
		success = false;
	}

	char leaderboardBtcPath[]= "images/leaderboard_background_btc.png";
	if( !gLeaderboardBtcTexture.loadFromFile( leaderboardBtcPath ) )
	{
		printf( "Failed to load leaderboard background btc texture!\n" );
		success = false;
	}

	char leaderboardBgPath[]= "images/leaderboard_background.png";

	if( !gLeaderboardBackgroundTexture.loadFromFile( leaderboardBgPath ) )
	{
		printf( "Failed to load leaderboard background texture!\n" );
		success = false;
	}

	char pauseBgPath[]="images/pause_background.png";

	if( !gPauseBackgroundTexture.loadFromFile( pauseBgPath ) )
	{
		printf( "Failed to load pause menu background texture!\n" );
		success = false;
	}

	char loginBgPath[]="images/login_background.png";

	if( !gLoginBackgroundTexture.loadFromFile( loginBgPath ) )
	{
		printf( "Failed to load login menu background texture!\n" );
		success = false;
	}

	char playPath[]="images/play_button.png";

    if( !gButtonPlayTexture.loadFromFile( playPath ) )
    {
        printf( "Failed to load play button texture!\n" );
        success = false;
    }

	char helpPath[]="images/help_button.png";

    if( !gButtonHelpTexture.loadFromFile( helpPath ) )
    {
        printf( "Failed to load help button texture!\n" );
        success = false;
    }

	char exitPath[]="images/exit_button.png";

    if( !gButtonExitTexture.loadFromFile( exitPath ) )
    {
        printf( "Failed to load exit button texture!\n" );
        success = false;
    }

	char leaderboardPath[]="images/leaderboard_button.png";

    if( !gButtonLeaderboardTexture.loadFromFile( leaderboardPath ) )
    {
        printf( "Failed to load leaderboard button texture!\n" );
        success = false;
    }

	char mode8ballPath[]="images/8ball_pool_button.png";

    if( !gButton8BallTexture.loadFromFile( mode8ballPath ) )
    {
        printf( "Failed to load 8 ball pool button texture!\n" );
        success = false;
    }

	char mode9ballPath[]="images/9ball_pool_button.png";

    if( !gButton9BallTexture.loadFromFile( mode9ballPath ) )
    {
        printf( "Failed to load 9 ball pool button texture!\n" );
        success = false;
    }

	char modeReplicationPath[]="images/replication_button.png";

    if( !gButtonReplicationTexture.loadFromFile( modeReplicationPath ) )
    {
        printf( "Failed to load replication button texture!\n" );
        success = false;
    }

	char backPath[]="images/back_button.png";

    if( !gButtonBackTexture.loadFromFile( backPath ) )
    {
        printf( "Failed to load back button texture!\n" );
        success = false;
    }

    char returnPath[]="images/return_button.png";

    if(!gButtonReturnTexture.loadFromFile( returnPath )){
		printf( "Failed to load return button texture!\n" );
		success=false;
	}

	char btcPath[]="images/btc_button.png";

	if( !gButtonBtcTexture.loadFromFile( btcPath ) )
	{
		printf( "Failed to load beat the clock button texture!\n" );
		success = false;
	}

	char quitPath[]="images/quit_button.png";

	if( !gButtonQuitTexture.loadFromFile( quitPath ) )
	{
		printf( "Failed to load quit button texture!\n" );
		success = false;
	}

	char restartPath[]="images/restart_button.png";

	if( !gButtonRestartTexture.loadFromFile( restartPath ) )
	{
		printf( "Failed to load restart button texture!\n" );
		success = false;
	}

	char returnGamePath[]="images/return_game_button.png";

	if( !gButtonReturnGameTexture.loadFromFile( returnGamePath ) )
	{
		printf( "Failed to load return to game button texture!\n" );
		success = false;
	}

	char introductionPath[]= "images/introduction_button.png";

	if( !gButtonIntroductionTexture.loadFromFile( introductionPath ) )
	{
		printf( "Failed to load introduction button texture!\n" );
		success = false;
	}

	char controlsPath[]= "images/controls_button.png";

	if( !gButtonControlsTexture.loadFromFile( controlsPath ) )
	{
		printf( "Failed to load controls button texture!\n" );
		success = false;
	}

	char rulesPath[]= "images/rules_button.png";

	if( !gButtonRulesTexture.loadFromFile( rulesPath ) )
	{
		printf( "Failed to load rules button texture!\n" );
		success = false;
	}

	char nextPath[]= "images/next_button.png";

	if( !gButtonNextTexture.loadFromFile( nextPath ) )
	{
		printf( "Failed to load next button texture!\n" );
		success = false;
	}

	char menuPath[]="images/menu_button.png";

	if( !gButtonMenuTexture.loadFromFile( menuPath ) )
	{
		printf( "Failed to load menu button texture!\n" );
		success = false;
	}

	char changePath[]="images/change_sides_button.png";

	if( !gButtonChangeSidesTexture.loadFromFile( changePath ) )
	{
		printf( "Failed to load change sides button texture!\n" );
		success = false;
	}

	char player1BallPath[]="images/player1_ball.png";

	if( !gButtonPlayer1BallTexture.loadFromFile( player1BallPath ) )
	{
		printf( "Failed to load player 1 ball button texture!\n" );
		success = false;
	}

	char player2BallPath[]="images/player2_ball.png";

	if( !gButtonPlayer2BallTexture.loadFromFile( player2BallPath ) )
	{
		printf( "Failed to load player 2 ball button texture!\n" );
		success = false;
	}

	char textBoxPath[]="images/text_box.png";

	if( !gTextBoxTexture.loadFromFile( textBoxPath ) )
	{
		printf( "Failed to load text box texture!\n" );
		success = false;
	}

	for(int id=0;id<2;id++){
		char rectLoginPath[]= "images/rect_login_button0.png";
		char rectRegisterPath[]= "images/rect_register_button0.png";
		char rectBackPath[]= "images/rect_back_button0.png";
		char rectCreatePath[]= "images/rect_create_button0.png";
		char rectEnterPath[]= "images/rect_enter_button0.png";
		char rectProceedPath[]= "images/rect_proceed_button0.png";
		char rectLogoutPath[]= "images/rect_logout_button0.png";
		char rectChangeSidesPath[]= "images/rect_change_sides_button0.png";
		char rectPlayGuestPath[]= "images/rect_play_as_guest_button0.png";

		if(id){
			rectLoginPath[24]='1';
			rectRegisterPath[27]='1';
			rectBackPath[23]='1';
			rectCreatePath[25]='1';
			rectEnterPath[24]='1';
			rectProceedPath[26]='1';
			rectLogoutPath[25]='1';
			rectChangeSidesPath[31]='1';
			rectPlayGuestPath[32]='1';
		}

		if( !gRectButtonLoginTexture[id].loadFromFile( rectLoginPath ) )
		{
			printf( "Failed to load rect login button texture!\n" );
			success = false;
		}

		if( !gRectButtonRegisterTexture[id].loadFromFile( rectRegisterPath ) )
		{
			printf( "Failed to load rect register button texture!\n" );
			success = false;
		}

		if( !gRectButtonBackTexture[id].loadFromFile( rectBackPath ) )
		{
			printf( "Failed to load rect back button texture!\n" );
			success = false;
		}

		if( !gRectButtonCreateTexture[id].loadFromFile( rectCreatePath ) )
		{
			printf( "Failed to load rect create button texture!\n" );
			success = false;
		}

		if( !gRectButtonEntertexture[id].loadFromFile( rectEnterPath ) )
		{
			printf( "Failed to load rect enter button texture!\n" );
			success = false;
		}

		if( !gRectButtonProceedTexture[id].loadFromFile( rectProceedPath ) )
		{
			printf( "Failed to load rect proceed button texture!\n" );
			success = false;
		}

		if( !gRectButtonLogoutTexture[id].loadFromFile( rectLogoutPath ) )
		{
			printf( "Failed to load rect logout button texture!\n" );
			success = false;
		}

		if( !gRectButtonChangeSidesTexture[id].loadFromFile( rectChangeSidesPath ) )
		{
			printf( "Failed to load rect change sides button texture!\n" );
			success = false;
		}

		if( !gRectButtonPlayAsGuestTexture[id].loadFromFile( rectPlayGuestPath ) )
		{
			printf( "Failed to load rect login button texture!\n" );
			success = false;
		}
	}



	char nameInputPath[]= "images/name_input.png";
	if( !gNameInputTexture.loadFromFile( nameInputPath ) )
	{
		printf( "Failed to load name input texture!\n" );
		success = false;
	}

	char cueHoverPath[]= "images/cue_hover .png";
    if( !gCueHoverTexture.loadFromFile( cueHoverPath ) )
	{
		printf( "Failed to load cue hover texture!\n" );
		success = false;
	}

	char boardPath[]= "images/board.png";
	if( !gBoardTexture.loadFromFile( boardPath ) )
	{
		printf( "Failed to load board texture!\n" );
		success = false;
	}

	char ballPath[]= "images/ball00.png";

	for(int ballId=0;ballId<16;ballId++){
		ballPath[12]='0'+ ballId%10;
		if(ballId>9)ballPath[11]='1';

		if( !gBallsTexture[ballId].loadFromFile( ballPath ) )
		{
			printf( "Failed to load ball %d texture!\n",ballId );
			success = false;
		}
	}

	char modifiedBallPath[]="images/modified_ball0#.png";

	for(int id=1;id<16;id++){
		char ch='0'+id%10;
		modifiedBallPath[21]=ch;
		if(id>9)modifiedBallPath[20]='1';

		if( !gModifiedBallsTexture[id].loadFromFile( modifiedBallPath ) )
		{
			printf( "Failed to load modified ball %d texture!\n",id );
			success = false;
		}
	}

	char cuePath[]= "images/cue2.png";
	if(!gCueTexture.loadFromFile(cuePath)){
        printf( "Failed to load cue texture!\n" );
		success = false;
	}

	char cueTriggerPath[]= "images/cue .png";
	if(!gCueTriggerTexture.loadFromFile( cueTriggerPath )){
        printf( "Failed to load cue speed trigger texture!\n" );
		success = false;
	}

	char bg1Path[]= "images/background1.png";
	if(!gBackgroundTexture[0].loadFromFile( bg1Path )){
        printf( "Failed to load background1 texture!\n" );
		success = false;
	}

	char smallPipePath[]= "images/smallpipe.png";
	if(!gSmallpipeTexture.loadFromFile( smallPipePath )){
        printf( "Failed to load small pipe texture!\n" );
		success = false;
	}

	char largePipePath[]= "images/largepipe.png";
	if(!gLargepipeTexture.loadFromFile( largePipePath )){
        printf( "Failed to load large pipe texture!\n" );
		success = false;
	}

	char timerPath[]= "images/timer_rect.png";
	if(!gTimerTexture.loadFromFile( timerPath )){
        printf( "Failed to load timer texture!\n" );
		success = false;
	}

	char modifiedPipePath[]="images/modified_large_pipe.png";

	if(!gModifiedLargePipeTexture.loadFromFile(modifiedPipePath)){
        printf( "Failed to load modified large pipe texture!\n" );
		success = false;
	}

	char ballPenaltyPath[]="images/ball_penalty_board.png";

	if(!gBallPenaltyBoardTexture.loadFromFile(ballPenaltyPath)){
        printf( "Failed to load ball penalty board texture!\n" );
		success = false;
	}

	for(int id=0;id<3;id++){
		char rules8ballPath[]="images/rules_8ball#.png";
		char rules9ballPath[]="images/rules_9ball#.png";
		char rulesBtcPath[]="images/rules_btc#.png";
		char rulesReplicationPath[]="images/rules_replication#.png";
		char introPath[]="images/introduction#.png";
		char controlsPath[]="images/controls#.png";

		rules8ballPath[18]='0'+id;
		rules9ballPath[18]='0'+id;
		rulesBtcPath[16]='0'+id;
		rulesReplicationPath[24]='0'+id;
		introPath[19]='0'+id;
		controlsPath[15]='0'+id;

		if(!gRules8ballBgTexture[id].loadFromFile(rules8ballPath)){
			printf( "Failed to load rules 8ball background texture %d!\n",id+1 );
			success = false;
		}

		if(!gRules9ballBgTexture[id].loadFromFile(rules9ballPath)){
			printf( "Failed to load rules 9ball background texture %d!\n",id+1 );
			success = false;
		}

		if(!gRulesBtcBgTexture[id].loadFromFile(rulesBtcPath)){
			printf( "Failed to load rules BTC background texture %d!\n",id+1 );
			success = false;
		}

		if(id<2 && !gRulesReplicationBgTexture[id].loadFromFile(rulesReplicationPath)){
			printf( "Failed to load rules replication background texture %d!\n",id+1 );
			success = false;
		}

		if(!gIntroductionBgTexture[id].loadFromFile(introPath)){
			printf( "Failed to load introduction background texture %d!\n",id+1 );
			success = false;
		}

		if(id<2 && !gControlsBgTexture[id].loadFromFile(controlsPath)){
			printf( "Failed to load controls background texture %d!\n",id+1 );
			success = false;
		}
	}

	//Load music
	char bgMusicPath[] = "sound/background_music.mp3";
    gBackgroundMusic = Mix_LoadMUS( bgMusicPath );
    if( gBackgroundMusic == NULL )
    {
        printf( "Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    //Load sound effects
    char ballBallHardPath[]="sound/ballhitsballhard.wav";
    gBallHitsBallHard = Mix_LoadWAV( ballBallHardPath );
    if( gBallHitsBallHard == NULL )
    {
        printf( "Failed to load ball hits ball hard sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    char ballBallSoftPath[]="sound/ballhitsballsoft.wav";
    gBallHitsBallSoft = Mix_LoadWAV( ballBallSoftPath );
    if( gBallHitsBallSoft == NULL )
    {
        printf( "Failed to load ball hits ball soft sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    char cueBallPath[]="sound/cuehitsball.wav";
    gCueHitsBall = Mix_LoadWAV( cueBallPath );
    if( gCueHitsBall == NULL )
    {
        printf( "Failed to load cue hits ball sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    char cueBallHoverPath[]="sound/cuehitsballhover.wav";
    gCueHitsBallHover = Mix_LoadWAV( cueBallHoverPath );
    if( gCueHitsBallHover == NULL )
    {
        printf( "Failed to load cue hits ball hover sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    char groanPath[]="sound/groan.wav";
    gGroan = Mix_LoadWAV( groanPath );
    if( gGroan == NULL )
    {
        printf( "Failed to load groan sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    char clapPath[]="sound/clap.wav";
    gClap = Mix_LoadWAV( clapPath );
    if( gClap == NULL )
    {
        printf( "Failed to load clap sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    char crowdCheerPath[]="sound/crowdcheer.wav";
    gCrowdCheer = Mix_LoadWAV( crowdCheerPath );
    if( gCrowdCheer == NULL )
    {
        printf( "Failed to load crowd cheer sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    char ballPocketedPath[]="sound/ballpocketed.wav";
    gBallPocketed = Mix_LoadWAV( ballPocketedPath );
    if( gBallPocketed == NULL )
    {
        printf( "Failed to load ball pocketed sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

	return success;
}

void close()
{
	//Free loaded images
	gMenu1BackgroundTexture.free();
	gPlayMenuBackgroundTexture.free();
	gPauseBackgroundTexture.free();

    gButtonPlayTexture.free();
    gButtonHelpTexture.free();
    gButtonLeaderboardTexture.free();
    gButtonExitTexture.free();
    gButton8BallTexture.free();
    gButton9BallTexture.free();
    gButtonReplicationTexture.free();
    gButtonBackTexture.free();
    gButtonReturnTexture.free();
    gButtonReturnGameTexture.free();
    gButtonRestartTexture.free();
    gButtonQuitTexture.free();
    gButtonMenuTexture.free();
    gButtonChangeSidesTexture.free();
    gButtonPlayer1BallTexture.free();
    gButtonPlayer2BallTexture.free();
    gButtonIntroductionTexture.free();
    gButtonControlsTexture.free();
    gButtonRulesTexture.free();

    gTextBoxTexture.free();
    for(int id=0;id<2;id++){
		gRectButtonLoginTexture[id].free();
		gRectButtonRegisterTexture[id].free();
		gRectButtonBackTexture[id].free();
		gRectButtonCreateTexture[id].free();
		gRectButtonEntertexture[id].free();
		gRectButtonProceedTexture[id].free();
		gRectButtonLogoutTexture[id].free();
		gRectButtonChangeSidesTexture[id].free();
		gRectButtonPlayAsGuestTexture[id].free();
	}

	gNameInputTexture.free();
	gBoardTexture.free();
	for(int i=0;i<16;i++)gBallsTexture[i].free();
	gCueTexture.free();
	for(int i=0;i<3;i++)gBackgroundTexture[i].free();
	gTimerTexture.free();
	gSmallpipeTexture.free();
	gLargepipeTexture.free();
	gCueHoverTexture.free();
	gTextPlayerTexture[1].free();
    gTextPlayerTexture[2].free();
    gTextWinTexture.free();
    gTextCongratsTexture.free();
    for(int i=0;i<62;i++)gTextTimeTexture[i].free();
    gTextPenaltytexture.free();
    gTextRewardtexture.free();

    gLeaderboardReplicationTexture.free();
    gLeaderboardBtcTexture.free();
    gLeaderboardBackgroundTexture.free();
    for(int recId=0;recId<LEADERBOARD_SIZE;recId++){
		gRecordNameTexture[recId].free();
		gRecordDurationTexture[recId].free();
	}
	for(int id=1;id<16;id++)gModifiedBallsTexture[id].free();
	gModifiedLargePipeTexture.free();
	gBallPenaltyBoardTexture.free();
	gTextWinMessageTexture.free();
	gTextBallPenaltyTexture.free();
	for(int i=0;i<TOTAL_FAILURE_TEXTS;i++)gTextFailureTexture[i].free();
	for(int i=0;i<TOTAL_TIPS;i++)gTextTipsTexture[i].free();

	gInputTextTexture.free();

	for(int i=0;i<3;i++){
		gRules8ballBgTexture[i].free();
		gRules9ballBgTexture[i].free();
		gRulesBtcBgTexture[i].free();
		if(i<2)gRulesReplicationBgTexture[i].free();
		gIntroductionBgTexture[i].free();
		if(i<2)gControlsBgTexture[i].free();
	}

	//Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;
    TTF_CloseFont(gFontTimer);
    gFontTimer=NULL;
    gFontBallPenalty=NULL;
    gFontTextInput=NULL;
    gFontFailure=NULL;
    gFontTips=NULL;
    gFontResult=NULL;
    gFontBtcEffect=NULL;

    //Free the music
    Mix_FreeMusic( gBackgroundMusic );
    gBackgroundMusic = NULL;
    //Free the sound effects
    Mix_FreeChunk( gBallHitsBallHard );
    Mix_FreeChunk( gBallHitsBallSoft );
    Mix_FreeChunk( gCueHitsBall );
    Mix_FreeChunk( gCueHitsBallHover );
    Mix_FreeChunk( gCrowdCheer );
    Mix_FreeChunk( gClap );
    Mix_FreeChunk( gGroan );
    Mix_FreeChunk( gBallPocketed );
    gBallHitsBallHard = NULL;
    gBallHitsBallSoft = NULL;
    gCueHitsBall = NULL;
    gCueHitsBallHover = NULL;
    gCrowdCheer = NULL;
    gClap = NULL;
    gGroan = NULL;
    gBallPocketed = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

