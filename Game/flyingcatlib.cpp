#include"flyingcatlib.h"
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 679;

std::string toString(int i){
    std:: stringstream ss;
    ss<<i;
    return ss.str(); 
}

void setRainbow(LRainbow *flyingCatRainbow){
    flyingCatRainbow[0].mPosX=0;
    flyingCatRainbow[0].mVelY=7.5;

    flyingCatRainbow[1].mPosX=flyingCatRainbow[0].mPosX+flyingCatRainbow[0].RAINBOW_WIDTH;
    flyingCatRainbow[1].mVelY=8.5;
    flyingCatRainbow[1].rainbowMotion=4;

    flyingCatRainbow[2].mPosX=flyingCatRainbow[1].mPosX+flyingCatRainbow[1].RAINBOW_WIDTH;
    flyingCatRainbow[2].mVelY=9.5;

    flyingCatRainbow[3].mPosX=flyingCatRainbow[2].mPosX+flyingCatRainbow[2].RAINBOW_WIDTH;
    flyingCatRainbow[3].mVelY=10.5;
    flyingCatRainbow[3].rainbowMotion=4;

    flyingCatRainbow[4].mPosX=flyingCatRainbow[3].mPosX+flyingCatRainbow[3].RAINBOW_WIDTH;
    flyingCatRainbow[4].mVelY=11.5;
}

