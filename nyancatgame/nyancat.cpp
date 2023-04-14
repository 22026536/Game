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

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 679;

class LTexture{
    public:
        LTexture();

        ~LTexture();

        bool loadFromFile( std::string path);

        bool loadFromRenderedText( std::string textureText, SDL_Color textColor, int Font );

        void free();

        void render(int x, int y, double angle = 0.0 ,SDL_Rect* clip = NULL,  SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

        int getWidth();
        int getHeight();

    private:

        SDL_Texture* mTexture;

        int mWidth;
        int mHeight;
};

class LCat{
    public:
        static const int CATIMAGE_WIDTH = 222;
        static const int CAT_WIDTH = 100 ;
        static const int CAT_HEIGHT = 80 ;

        int lineToRender;

        LCat();

        void handleEvent (SDL_Event& e);

        double distance(double x1,double y1,double x2, double y2);

        bool check(int mFoodX, int mFoodWidth, int mFoodLine);

        void render(int i);
    
    private:
    const int mPosX=50;
    int mPosY;
    int mVelY_;
    double mVelY;

};

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

        void render(int food);
    private:
};

enum nyanCatFood{
    strawberry,
    cupcake,
    cookie,
    cake,
    peppermint,
    bomb,
    totalFood,
};

bool init();

bool loadMedia();

void close();


SDL_Window *gWindow = NULL;

TTF_Font *gFont=NULL;

SDL_Renderer *gRenderer=NULL;

LTexture backGround;
LTexture catFood[totalFood];
LTexture cat[3];
LTexture TapToStart;
LTexture GameOver;
LTexture Score; 

Mix_Chunk *jump=NULL;

LTexture::LTexture(){
    mTexture=NULL;
    mWidth=0;
    mHeight=0;
}

LTexture::~LTexture(){
    free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 52, 102 ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor,int Font )
{
	//Get rid of preexisting texture
	free();
    gFont = TTF_OpenFont( "E:/NewFolder/font.ttf", Font );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	
	//Return success
	return mTexture != NULL;
}

void LTexture::free(){
    //Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y,double angel,SDL_Rect* clip, SDL_Point* center, SDL_RendererFlip flip){

    SDL_Rect renderQuad = {x,y,mWidth,mHeight};

    if(clip!=NULL){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad,angel,center,flip);
}

int LTexture::getWidth(){
    return mWidth;
}

int LTexture::getHeight(){
    return mHeight;
}


LCat::LCat()
{
    //Initialize the offsets
    lineToRender=3;
    //Initialize the velocity
    mVelY = 0;
    mVelY_=0;
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

void LCat::render(int i)
{
    //Show the dot
    mPosY=(SCREEN_HEIGHT/7*lineToRender)-CAT_HEIGHT/2;
	cat[i].render( mPosX, mPosY);
}

bool LCat::check(int mFoodX,int mFoodWidth, int mFoodLine){
    int mCatX=mPosX+CATIMAGE_WIDTH-CAT_WIDTH;
    if(lineToRender!=mFoodLine) return false;
    if(mCatX+CAT_WIDTH<mFoodX) return false;
    if(mCatX>mFoodX+mFoodWidth) return false;
    Mix_PlayChannel(-1,jump,0);
    return true;
}

LFood::LFood(){
    mPosX=-300;
    mPosY=0;
    foodToRender=0;
    lineToRender=0;
}

void LFood::render(int food){
    mPosY=(SCREEN_HEIGHT/7*lineToRender)-FOOD_HEIGHT/2;
    catFood[food].render(mPosX,mPosY);
    if(mPosX>-300) mPosX-=mVelX;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}

                if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia(){
    bool success=true;

    if( !backGround.loadFromFile( "E:/nyancatgame/nyancatbackground.jpg" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}

    if( !cat[0].loadFromFile( "E:/nyancatgame/nyancat1.png" ) )
	{
		printf( "Failed to load cat texture!\n" );
		success = false;
	}
    if( !cat[1].loadFromFile( "E:/nyancatgame/nyancat2.png" ) )
	{
		printf( "Failed to load cat texture!\n" );
		success = false;
	}
    if( !cat[2].loadFromFile( "E:/nyancatgame/nyancat3.png" ) )
	{
		printf( "Failed to load cat texture!\n" );
		success = false;
	}
    
    if( !catFood[strawberry].loadFromFile("E:/nyancatgame/strawberry.png")){
        printf("Failed to load food texture!\n");
        success=false;
    }
    if( !catFood[cupcake].loadFromFile("E:/nyancatgame/cupcake.png")){
        printf("Failed to load food texture!\n");
        success=false;
    }
    if( !catFood[cookie].loadFromFile("E:/nyancatgame/cookie.png")){
        printf("Failed to load food texture!\n");
        success=false;
    }
    if( !catFood[cake].loadFromFile("E:/nyancatgame/cake.png")){
        printf("Failed to load food texture!\n");
        success=false;
    }
    if( !catFood[peppermint].loadFromFile("E:/nyancatgame/peppermint.png")){
        printf("Failed to load food texture!\n");
        success=false;
    }
    if( !catFood[bomb].loadFromFile("E:/nyancatgame/bomb.png")){
        printf("Failed to load food texture!\n");
        success=false;
    }
    jump=Mix_LoadWAV("E:/Newfolder/jump.wav");
    if( jump == NULL )
	{
		printf( "Failed to load jump sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

		//Render text
	SDL_Color textColor = { 0,0,0};
	if( !TapToStart.loadFromRenderedText( "TAP TO START", textColor,50 ) )
	{
		printf( "Failed to render text texture!\n" );
		success = false;
	}

    return success;
}

std::string toString(int i){
    std:: stringstream ss;
    ss<<i;
    return ss.str(); 
}
void close(){
    backGround.free();
    cat[0].free();
    cat[1].free();
    cat[2].free();
    for(int i=0;i<totalFood;i++){
        catFood[i].free();
    }
    TapToStart.free();
    Score.free();
    GameOver.free();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer=NULL;
    gWindow=NULL;

    TTF_CloseFont(gFont);
    gFont=NULL;

    IMG_Quit();
    SDL_Quit();
    Mix_Quit();
    TTF_Quit();
}

double LCat::distance(double x1,double y1, double x2,double y2){
    double distanceX=(x1-x2)*(x1-x2);
    double distanceY=(y1-y2)*(y1-y2);
    double distance=distanceX+distanceY;
    return distance;
}

int main(int argc,char*argv[]){
    if( !init() ){
        printf("Failed to initialize!\n");
    }
    else{
        if(!loadMedia()){
            printf("Failed to load media!\n");
        }
        else{
            a:;
            bool quit=false;

            bool start=false;

            SDL_Event e;

            LCat nyanCat;

            LFood food[30];

            SDL_Color textColor={0,0,0};

            std::string SCORETEXT;

            int SCORE = 0;

            int scrollingOffset=0;

            int scrollingOffsetVel=2;

            int spawnFood=0;

            int cat=0;

            bool endGame =false;
                while( !endGame ){
                    while(SDL_PollEvent(&e)!=0){
                        if(e.type==SDL_QUIT){
                            quit=true;
                            endGame=true;
                        }
                        nyanCat.handleEvent(e);
                    }
                    
                    scrollingOffset-=scrollingOffsetVel;

                    if(scrollingOffset<-backGround.getWidth()){
                        scrollingOffset=0;
                    }

                    SDL_SetRenderDrawColor(gRenderer,255,255,255,255);
                    SDL_RenderClear(gRenderer);

                    backGround.render(scrollingOffset,0,0.0);
                    backGround.render(scrollingOffset+backGround.getWidth(),0,0.0);

                    SDL_SetRenderDrawColor(gRenderer,255,255,255,255);
                    for(int i=1;i<=6;i++){
                    SDL_RenderDrawLine(gRenderer,0,SCREEN_HEIGHT/7*i,SCREEN_WIDTH,SCREEN_HEIGHT/7*i);
                    }
                    if(spawnFood==0){
                        for(int i=0;i<10;++i){
                            food[i].mPosX=SCREEN_WIDTH+90*i;;
                            food[i].lineToRender=rand()%6+1;
                            food[i].foodToRender=abs(rand())%totalFood;
                        }
                    }
                    if(spawnFood==150){
                        for(int i=10;i<20;++i){
                            food[i].mPosX=SCREEN_WIDTH+90*(i-10);;
                            food[i].lineToRender=rand()%6+1;
                            food[i].foodToRender=abs(rand())%totalFood;
                        }
                    }
                    if(spawnFood==300){
                        for(int i=20;i<30;++i){
                            food[i].mPosX=SCREEN_WIDTH+90*(i-20);;
                            food[i].lineToRender=rand()%6+1;
                            food[i].foodToRender=rand()%totalFood;
                        }
                    }
                    for(int i=0;i<30;++i){
                        if(food[i].foodToRender==bomb) food[i].mVelX=10;
                        food[i].render(food[i].foodToRender);
                        if(nyanCat.check(food[i].mPosX,85,food[i].lineToRender)==true){
                            if(food[i].foodToRender==bomb) endGame=true;
                            food[i].mPosX=-300;
                            SCORE++;
                        }
                    }
                    spawnFood++;
                    if(spawnFood==450) spawnFood=0;

                    nyanCat.render(cat/3);
                    cat++;
                    if(cat>6) cat=0;

                    Score.free();
                    SCORETEXT="SCORE: ";
                    SCORETEXT+=toString(SCORE);
                    Score.loadFromRenderedText(SCORETEXT.c_str(),textColor,20);

                    Score.render(0,0,0);

                    SDL_RenderPresent(gRenderer);
                    SDL_Delay(25);
                }
                while(!quit){
                    while(SDL_PollEvent(&e)!=0){
                        if(e.type==SDL_QUIT){
                            quit=true;
                        }
                    }
                    SDL_SetRenderDrawColor(gRenderer,255,255,255,255);
                    SDL_RenderClear(gRenderer);

                    backGround.render(scrollingOffset,0,0.0);
                    backGround.render(scrollingOffset+backGround.getWidth(),0,0.0);

                    SDL_SetRenderDrawColor(gRenderer,255,255,255,255);
                    for(int i=1;i<=6;i++){
                    SDL_RenderDrawLine(gRenderer,0,SCREEN_HEIGHT/7*i,SCREEN_WIDTH,SCREEN_HEIGHT/7*i);
                    }

                    for(int i=0;i<30;++i){
                        food[i].mVelX=0;
                        food[i].render(food[i].foodToRender);
                    }

                    nyanCat.render(cat/3);

                    SDL_RenderPresent(gRenderer);
                    SDL_Delay(25);
                }
        }
    }

    close();
    return 0;
}
