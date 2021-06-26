#ifndef TIME_H
#define TIME_H

void zeroPadTime(char tm[]);

void formatTime(char tm[],int milliSecs);

void loadAndRenderElapsedTime();

void loadAndRenderRemainingTime(int totalTime);

void renderTime();

#endif
