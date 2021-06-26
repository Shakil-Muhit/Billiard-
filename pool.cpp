/*This is the main module containing the main funtion */
#include <stdio.h>
#include <SDL2/SDL.h>
#include "include/sdl_handler.h"
#include "include/window_handler.h"

//Main loop flag
int quit = 0;

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Event handler
			SDL_Event e;
            initAllWindowHandler();
			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
                    handleEventWindowHandler(e);
				}

                renderAllWindowHandler();
			}
		}
	}
	//Free resources and close SDL
	close();
	return 0;
}
