/* This module controls the virtual dash line to show the movement direction 
 * of the cue ball before it is hit, as well as the direction of movement 
 * of the ball to be hit by cue ball confined to a constant scale */

#include "include/vec.h"
#include "include/ball.h"
#include "include/aimline.h"
#include <SDL2/SDL.h>
#include "include/consts.h"
#include "include/maths.h"
#include "include/8_ball.h"
#include "include/9_ball.h"
#include "include/replication_mode.h"
#include "include/btc_mode.h"
#include "include/window_handler.h"

extern int ballType[16],playerType[2],totalScored[2],curPlayer;
extern SDL_Renderer* gRenderer;
extern int windowMode;

void renderAimline(Ball ball[16],Vec currentDirection,point origin,point rightBottom){
    double mn=1e8;
    point from,to;

    for(double mg=0.01;;mg+=0.01){
        point p= {origin.x+mg*currentDirection.x,origin.y+currentDirection.y*mg};
        if(p.y-ball[0].r<EXTRA_HEIGHT+WALL_WIDTH||p.y+ball[0].r>SCREEN_HEIGHT-WALL_WIDTH-EXTRA_HEIGHT_END)break;
        if(p.x-ball[0].r<EXTRA_WIDTH+WALL_WIDTH||p.x+ball[0].r>SCREEN_WIDTH-WALL_WIDTH-EXTRA_WIDTH_END)break;
        int done=0;
        for(int ballId=1;ballId<16;ballId++){
            if(dist(p,{ball[ballId].x,ball[ballId].y})<ball[0].r+ball[ballId].r){
                mn=mg;
                Vec changedDirection={ball[ballId].x-p.x,ball[ballId].y-p.y};

                changedDirection=scale(changedDirection,AIMLINE_SCALE);
                from={ball[ballId].x,ball[ballId].y};
                to=translate(from,changedDirection);

                if(windowMode==MODE_8BALL_WINDOW && isValidAim8ball(ballId))SDL_RenderDrawLine( gRenderer, from.x, from.y, to.x, to.y );
                else if(windowMode==MODE_9BALL_WINDOW && isValidAim9ball(ballId))SDL_RenderDrawLine( gRenderer, from.x, from.y, to.x, to.y );
                else if(windowMode==MODE_REPLICATION_WINDOW && isValidAimReplication(ballId))SDL_RenderDrawLine( gRenderer, from.x, from.y, to.x, to.y );
                else if(windowMode==MODE_BTC_WINDOW && isValidAimBtc(ballId))SDL_RenderDrawLine( gRenderer, from.x, from.y, to.x, to.y );
                done=1;
                break;
            }
        }
        if(done)break;
    }

    for(double mg=0.01;mg<mn;mg+=0.01){
        point p= {origin.x+mg*currentDirection.x,origin.y+currentDirection.y*mg};
        if(p.y-ball[0].r<EXTRA_HEIGHT+WALL_WIDTH||p.y+ball[0].r>SCREEN_HEIGHT-WALL_WIDTH-EXTRA_HEIGHT_END)break;
        if(p.x-ball[0].r<EXTRA_WIDTH+WALL_WIDTH||p.x+ball[0].r>SCREEN_WIDTH-WALL_WIDTH-EXTRA_WIDTH_END)break;
        if(distanceSquared((int)p.x,(int)p.y,(int)origin.x,(int)origin.y)<ball[0].r*ball[0].r)continue;
        SDL_RenderDrawPoint( gRenderer, p.x, p.y );
    }

}
