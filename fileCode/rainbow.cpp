#include"flyingcatlib.h"
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 679;

LRainbow::LRainbow(){
    mPosX=0;
    mPosY=(SCREEN_HEIGHT/7*3)-80/2;
    rainbowMotion=0;
    mVelY=1;
}