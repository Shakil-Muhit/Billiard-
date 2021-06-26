#ifndef PIPES_H
#define PIPES_H

struct ThreePipes{
    int small1x,small1y,small2x,small2y,largex,largey,smallHeight,smallWidth,largeHeight,largeWidth;

    void init(int x1,int y1,int x2,int y2,int x3,int y3);

    void render();
};

#endif
