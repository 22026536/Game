#include"flyingcatlib.h"
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 679;

LCat::LCat()
{
    mPosX=30;
    lineToRender=3;
    catMotion=0;

    //vị trí ban đầu
    mPosY=(SCREEN_HEIGHT/7*lineToRender)-CAT_HEIGHT/2;
    mPosY_ =(SCREEN_HEIGHT/7*lineToRender)-CAT_HEIGHT/2;
}

void LCat::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if(e.type==SDL_KEYDOWN&&e.key.repeat==0){
        switch (e.key.keysym.sym){
        case SDLK_UP: lineToRender--;break;
        case SDLK_DOWN: lineToRender++;break;
        }
    }

    if(lineToRender<1) lineToRender=1;
    if(lineToRender>6) lineToRender=6;
}

bool LCat::isMove(){
    bool isMove=false;
    if(mPosY!=mPosY_) isMove=true;
    return isMove;
}

double LCat::distance(double x1,double y1, double x2,double y2){
    double distanceX=(x1-x2)*(x1-x2);
    double distanceY=(y1-y2)*(y1-y2);
    double distance=distanceX+distanceY;
    return distance;
}

bool LCat::check(int mFoodX,int mFoodWidth, int mFoodLine){
    int mCatX=mPosX+CAT_IMAGE_WIDTH-CAT_WIDTH;
    if(lineToRender!=mFoodLine) return false;
    if(mCatX+CAT_WIDTH<mFoodX) return false;
    if(mCatX>mFoodX+mFoodWidth) return false;
    return true;
}

