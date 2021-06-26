/* This module controls the records of the leaderboard
 * of single player modes */

#include <SDL2/SDL.h>
#include "include/sdl_handler.h"
#include "include/record.h"
#include "include/consts.h"
#include <stdio.h>

extern TTF_Font *gFont;

extern LTexture gRecordNameTexture[LEADERBOARD_SIZE];
extern LTexture gRecordDurationTexture[LEADERBOARD_SIZE];
extern LTexture gBallsTexture[16];

void Record::assign(Record rec){
	int len=0;
	while(rec.name[len]){
		name[len]=rec.name[len];
		len++;
	}
	name[len]='\0';

	len=0;
	while(rec.durationText[len]){
		durationText[len]=rec.durationText[len];
		len++;
	}
	durationText[len]='\0';
	duration=rec.duration;
}

Record recData[LEADERBOARD_SIZE];

void writeData(char path[]){
	SDL_RWops* file = SDL_RWFromFile( path, "r+b" );

	file = SDL_RWFromFile( path, "w+b" );
	if( file != NULL )
	{
		//copy data
		for( int i = 0; i < LEADERBOARD_SIZE; ++i )
		{
			SDL_RWwrite( file, &recData[ i ], sizeof(Record), 1 );
		}

		//Close file handler
		SDL_RWclose( file );
	}
	else
	{
		printf( "Error: Unable to create file! SDL Error: %s\n", SDL_GetError() );
	}

}

void initRecord(char path[],int neutralDuration){
	Record neutral={"-","-",neutralDuration};

	//Open file for reading in binary
    SDL_RWops* file = SDL_RWFromFile( path, "r+b" );

	//File does not exist
    if( file == NULL )
    {
	printf( "Warning: Unable to open file! SDL Error: %s\n", SDL_GetError() );

	//Create file for writing
	file = SDL_RWFromFile( path, "w+b" );

		if( file != NULL )
        {
            printf( "New file created!\n" );

            //Initialize data
            for( int i = 0; i < LEADERBOARD_SIZE; ++i )
            {
                recData[ i ] = neutral;
                SDL_RWwrite( file, &recData[ i ], sizeof(Record), 1 );
            }

            //Close file handler
            SDL_RWclose( file );
        }
        else
        {
            printf( "Error: Unable to create file! SDL Error: %s\n", SDL_GetError() );
        }
    }
    //File exists
    else
    {
        //Load data
        printf( "Reading file...!\n" );
        for( int i = 0; i < LEADERBOARD_SIZE; ++i )
        {
            SDL_RWread( file, &recData[ i ], sizeof(Record), 1 );
        }

        //Close file handler
        SDL_RWclose( file );
    }

    SDL_Color textColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    //Initialize data textures
    for( int i = 0; i < LEADERBOARD_SIZE; ++i )
    {
        gRecordNameTexture[ i ].loadFromRenderedText( recData[ i ].name, textColor, gFont );
		gRecordDurationTexture[ i ].loadFromRenderedText( recData[ i ].durationText, textColor, gFont );
    }

}

void insertRecord(int pos,char name[],char durationText[],int duration){
	for(int recId=LEADERBOARD_SIZE-1;recId>pos;recId--){
		//rec_data[rec_id]={rec_data[rec_id-1].name,rec_data[rec_id-1].duration_text,rec_data[rec_id-1].duration};
		recData[recId].assign(recData[recId-1]);
	}
	//Record cur={name,duration_text,duration};
	//rec_data[pos].assign(cur);
	int len=0;
	while(name[len]){
		recData[pos].name[len]=name[len];
		len++;
	}
	recData[pos].name[len]='\0';

	len=0;
	while(durationText[len]){
		recData[pos].durationText[len]=durationText[len];
		len++;
	}
	recData[pos].durationText[len]='\0';
	recData[pos].duration=duration;
}

void renderRecord(){
	int x=73,y=165,gap=(SCREEN_HEIGHT-165-100)/10,xball=x/2;

	for(int recId=0;recId<LEADERBOARD_SIZE;recId++){
		int height= gBallsTexture[recId+1].getHeight();
		int width= gBallsTexture[recId+1].getWidth();
		int heightRec=gRecordNameTexture[recId].getHeight();
		gRecordNameTexture[recId].render(x,y);
		gBallsTexture[recId+1].render(xball-width/2,y+(heightRec-height)/2);
		y += gap;
	}

	x=883;
	y=165;

	for(int recId=0;recId<LEADERBOARD_SIZE;recId++){
		int width= gRecordDurationTexture[recId].getWidth();
		gRecordDurationTexture[recId].render(x-width/2,y);
		y += gap;
	}
	//printf("REC %d %s\n",rec_data[0].duration,rec_data[0].duration_text);
}
