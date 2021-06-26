/* This module controls the state of user data and 
 * controls user registration and authentication */

#include <SDL2/SDL.h>
#include "include/user.h"
#include "include/string.h"
#include "include/consts.h"

int totalUsers;
User userData[USER_LIMIT];

static char path[]="bin/users.bin";

void User::assign(User user){
	id= user.id;
	strAssign(name,user.name);
	strAssign(password,user.password);
}

void writeUserData(){
	SDL_RWops* file = SDL_RWFromFile( path, "r+b" );

	file = SDL_RWFromFile( path, "w+b" );
	if( file != NULL )
	{
		//copy data
		for( int i = 0; i < USER_LIMIT; ++i )
		{
			SDL_RWwrite( file, &userData[ i ], sizeof(User), 1 );
		}

		//Close file handler
		SDL_RWclose( file );
	}
	else
	{
		printf( "Error: Unable to create file! SDL Error: %s\n", SDL_GetError() );
	}

}

void initUser(){
	User neutral={-1,"-","-"};
	totalUsers=0;

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
            for( int i = 0; i < USER_LIMIT; ++i )
            {
                userData[ i ] = neutral;
                SDL_RWwrite( file, &userData[ i ], sizeof(User), 1 );
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
        for( int i = 0; i < USER_LIMIT; ++i )
        {
            SDL_RWread( file, &userData[ i ], sizeof(User), 1 );
            if(userData[i].id!=-1)totalUsers++;
        }

        //Close file handler
        SDL_RWclose( file );
    }

    SDL_Color textColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    ////Initialize data textures
    //for( int i = 0; i < LEADERBOARD_SIZE; ++i )
    //{
        //gRecordNameTexture[ i ].loadFromRenderedText( rec_data[ i ].name, textColor, gFont );
		//gRecordDurationTexture[ i ].loadFromRenderedText( rec_data[ i ].duration_text, textColor, gFont );
    //}

}

int addUser(char username[],char pass[]){
	char guest1[]= "Player 1",guest2[]= "Player 2";
	if(!strCompare(username,guest1)||!strCompare(username,guest2))return -2;
	int lnUser= strLen(username), lnPass= strLen(pass);
	if(!lnUser || !lnPass)return -1;

	for(int id=0;id<totalUsers;id++){
		if(!strCompare(username,userData[id].name))return 0;
	}

	userData[totalUsers].id= totalUsers;
	strAssign(userData[totalUsers].name,username);
	strAssign(userData[totalUsers].password,pass);
	totalUsers++;

	writeUserData();
	return 1;
}

int authenticate(char username[],char pass[],int &userId){
	for(int id=0;id<totalUsers;id++){
		if(!strCompare(username,userData[id].name)){
			if(!strCompare(pass,userData[id].password)){
				userId=id;
				return 1;
			}
			return 0;
		}
	}
	return -1;
}

