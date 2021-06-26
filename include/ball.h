#ifndef BALL_H
#define BALL_H
#include <SDL2/SDL.h>
#include "vec.h"

struct Ball{
    int height,width,id,isInsidePocket=1,isMoving,r,inside;
    double initVelx,initVely,velx,vely,x,y,magnitude;
    void init(int x1,int y1,int ind);
    int isInside(int x1,int y1,Ball ball);

    int isVacant(int x1,int y1,Ball ball[]);

    void placeBallRandomly(Ball &cueBall,Ball ball[]);

    void cueBallDrag(Ball &cueBall,SDL_Event &e,Ball ball[]);

    void setVectorVelocity(Vec direction,double mag);

    void recalcVel();

    void oneDimensionCollision(double vel1,double vel2,double &finalVel1,double &finalVel2);

    void collision2Ds(double m1, double m2, double R,
                  double x1, double y1, double x2, double y2,
                  double& vx1, double& vy1, double& vx2, double& vy2,Ball &ball1,Ball &ball2);



    void handle2DCollision(Ball &ball1,Ball &ball2,Ball ball[]);

    void move(Ball ball[]);

    void render();
};


#endif
