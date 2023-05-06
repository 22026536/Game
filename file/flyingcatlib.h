#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<mixer.h>
#include<SDL_ttf.h>
#include<stdio.h>
#include<string>
#include<sstream>
#include<iostream>
#include<cstdlib>
#include<time.h>

//class load ảnh
class LTexture{
    public:
        LTexture();

        ~LTexture();

        bool loadFromFile( std::string path);

        bool loadFromRenderedText( std::string textureText, SDL_Color textColor, int Font );

        void free();

        //thay đổi màu của ảnh
        void setColor();

        //Set alpha 
        void setAlpha( Uint8 alpha );

        //render ảnh tĩnh
        void render(int x, int y, double angle = 0.0 ,SDL_Rect* clip = NULL,  SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
        
        //render ảnh động
        void renderMotion(int x,int y,int &motion);

        //check xem con chuột có ở trong ảnh không
        bool checkMouse();

        int mWidth;
        int mHeight;
        int mPosX;
        int mPosY;

    private:

        SDL_Texture* mTexture;
};

//class con mèo
class LCat{
    public:
        static const int CATIMAGE_WIDTH = 222;
        static const int CAT_WIDTH = 100 ;
        static const int CAT_HEIGHT = 80 ;

        int lineToRender;
        int catMotion;
        int mPosX;
        int mPosY;

        LCat();

        void handleEvent (SDL_Event& e);

        bool isMove();

        double distance(double x1,double y1,double x2, double y2);

        bool check(int mFoodX, int mFoodWidth, int mFoodLine);

        void render();

        void renderMotion();
    
    private:
    int mPosY_;
    const int mVelY=12;

};

//class đồ ăn
class LFood{
    public:
        const int FOOD_WIDTH=85;
        const int FOOD_HEIGHT=80;
        int mPosX;
        int mPosY;
        int mVelX=7;
        int foodToRender;
        int lineToRender;

        LFood();

        void render();

        void renderMotion();

    private:
};

//class cầu vòng sau con mèo
class LRainbow{
    public:
        int mPosX;
        int mPosY;
        int rainbowMotion;
        const int RAINBOW_WIDTH = 30;   
        double mVelY;
    
    LRainbow();
    
    void render();

    void renderMotion();

    private:
};

//hàm khởi tạo
bool init();

//hàm load ảnh
bool loadMedia();

//hàm đóng
void close();

//chuyen int ve string
std::string toString(int number);