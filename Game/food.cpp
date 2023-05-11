#include"flyingcatlib.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 679;

enum nyanCatFood{
    strawberry,
    cupcake,
    cookie,
    cake,
    peppermint,
    gumdrop,
    bomb,
    rocket,
    totalFood,
};

LFood::LFood(){
    mPosX = -300;
    mPosY = 0;

    foodToRender = 0;
    lineToRender = 0;

    choosed = false;
}

void LFood::chooseFood(int difficultVel){
    if(choosed == false){
        foodToRender = rand() % totalFood;
        lineToRender = rand() % 6 + 1;

        choosed = true;
    }

    mPosY=( SCREEN_HEIGHT / 7 * lineToRender ) - FOOD_HEIGHT / 2;

    if( foodToRender == bomb ) mVelX = 10 + difficultVel;
    else if( foodToRender == rocket ) mVelX = 13 + round( difficultVel * 1.5 );
    else mVelX = 7 + difficultVel;
}

void LFood::setFood(int startPosition){
    mPosX = SCREEN_WIDTH + startPosition * foodDistance;
}

