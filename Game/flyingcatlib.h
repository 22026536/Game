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
#include<cmath>

//class ảnh
class LTexture{
    public:
        int mWidth;
        int mHeight;
        int mPosX;
        int mPosY;

        LTexture();

        ~LTexture();

        void free();

        //load file ảnh
        bool loadFromFile( std::string path, SDL_Renderer *gRenderer );

        //tạo ảnh text
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor, SDL_Renderer *gRenderer, TTF_Font *gFont );

        //thay đổi màu của ảnh
        void setColor();

        //Set alpha ảnh
        void setAlpha( Uint8 alpha );

        //render ảnh
        void render( int x, int y, SDL_Renderer *gRenderer, double angle = 0.0 ,SDL_Rect* clip = NULL,  SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        //check xem con chuột có ở trong ảnh không
        bool checkMouse();

    private:
        SDL_Texture* mTexture;
};

//class con mèo
class LCat{
    public:
        static const int CAT_IMAGE_WIDTH = 222;
        static const int CAT_WIDTH = 100 ;
        static const int CAT_HEIGHT = 80 ;

        int lineToRender;
        int catMotion;
        int mPosX;
        int mPosY;

        LCat();

        //If a key was pressed
        void handleEvent ( SDL_Event& e );

        //is cat moving
        bool isMove();

        //check distance
        double distance( double x1, double y1, double x2, double y2);
        
        //check is eat food
        bool check( int mFoodX, int mFoodWidth, int mFoodLine );

        //render cat image
        void render( SDL_Renderer *gRenderer );

        //render cat image motion
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

        bool choosed;

        LFood();

        //choose food
        void chooseFood( int difficultVel );

        //set start position food
        void setFood( int startPosition );

        //render food image
        void render();

        //render food image motion
        void renderMotion( SDL_Renderer *gRenderer );

    private:
        const int totalRender = 30;
        const int foodDistance = 90;
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
    
    //render rainbow image
    void render();

    //render rainbow image motion
    void renderMotion();

    private:
};

//hàm khởi tạo
bool init();

//hàm load ảnh
bool loadMedia();

//hàm đóng
void close();

//set tốc độ của từng cầu vồng
void setRainbow( LRainbow flyingCatRainbow[] );

//set các thông số trước khi về menu
void toMenu();

//set các thông số trước khi restart
void toRestart();

//hàm chạy game khi chưa bắt đầu
void whileHaveNotStarted();

//hàm chạy khi dừng game
void whilePauseGame();

//hàm chạy khi chơi game
void whilePlayGame();

//hàm kiểm tra xem có được điểm cao nhất và cài đặt chữ khi kết thúc
void checkScore();

//hàm chạy khi kết thúc game
void whileEndGame( int &alpha );

//quit game
void toQuit();

//chuyen int ve string
std::string toString( int number );