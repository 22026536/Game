#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include<iostream>
//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

//Button constants
const int BUTTON_WIDTH = 210;
const int BUTTON_HEIGHT = 210;
const int TOTAL_BUTTONS = 9;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_X = 2,
	BUTTON_SPRITE_MOUSE_O =3
};

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();


		//The actual hardware texture
		SDL_Texture* mTexture;
	private:
		//Image dimensions
		int mWidth;
		int mHeight;
};

//The mouse button
class LButton
{
	public:
		//Initializes internal variables
		LButton();

		//Sets top left position
		void setPosition( int x, int y );

		//Handles mouse event
		bool handleEvent( SDL_Event* e ,bool turn);
	
		//Shows button sprite
		void render();

	private:
		//Top left position
		SDL_Point mPosition;

		//Currently used global sprite
		LButtonSprite mCurrentSprite;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

SDL_Surface *Draw = NULL;

SDL_Surface *XWin = NULL;

SDL_Surface *OWin = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Mouse button sprites
SDL_Rect gSpriteClips[ 4 ];
LTexture gButtonSpriteSheetTexture;

//Buttons objects
LButton gButtons[ TOTAL_BUTTONS +1]; 

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
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
		SDL_Rect rect;
		rect.x=0;
		rect.y=0;
		rect.w=BUTTON_WIDTH*2+15;
		rect.h=BUTTON_HEIGHT*2+15;
		SDL_BlitScaled(loadedSurface,NULL,loadedSurface,&rect);
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


void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
    }
		

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition( int x, int y )
{
	mPosition.x = x;
	mPosition.y = y;
}

bool LButton::handleEvent( SDL_Event* e ,bool turn)
{
	//If mouse event happened
	bool choose=false;
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + BUTTON_WIDTH )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + BUTTON_HEIGHT )
		{
			inside = false;
		}

		//Mouse is outside button
		if( !inside )
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;
		
				case SDL_MOUSEBUTTONUP:
				if(turn==false) mCurrentSprite = BUTTON_SPRITE_MOUSE_X;
				if(turn==true) mCurrentSprite = BUTTON_SPRITE_MOUSE_O;
				choose=true;
				break;
			}
		}
	}
	return choose;
}
	
void LButton::render()
{
	//Show current button sprite
	gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
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
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprites
	if( !gButtonSpriteSheetTexture.loadFromFile( "TicTacToeGame/tictactoe.png" ) )
	{
		printf( "Failed to load button sprite texture!\n" );
		success = false;
	}
	else
	{
		//Set sprites
			gSpriteClips[ 0 ].x = 0;
			gSpriteClips[ 0 ].y = 0;
			gSpriteClips[ 0 ].w = BUTTON_HEIGHT;
			gSpriteClips[ 0 ].h = BUTTON_WIDTH;

			gSpriteClips[ 1 ].x = BUTTON_HEIGHT+10;
			gSpriteClips[ 1 ].y = 0;
			gSpriteClips[ 1 ].w = BUTTON_WIDTH;
			gSpriteClips[ 1 ].h = BUTTON_HEIGHT;

			gSpriteClips[ 2 ].x = 0;
			gSpriteClips[ 2 ].y = BUTTON_WIDTH;
			gSpriteClips[ 2 ].w = BUTTON_HEIGHT;
			gSpriteClips[ 2 ].h = BUTTON_WIDTH;

			gSpriteClips[ 3 ].x = BUTTON_HEIGHT+11;
			gSpriteClips[ 3 ].y = BUTTON_WIDTH;
			gSpriteClips[ 3 ].w = BUTTON_HEIGHT;
			gSpriteClips[ 3 ].h = BUTTON_WIDTH;

		//Set buttons in corners
		gButtons[ 0 ].setPosition( 0, 0 );
		gButtons[ 1 ].setPosition( (SCREEN_HEIGHT-BUTTON_HEIGHT*3)/2+BUTTON_HEIGHT, 0 );
		gButtons[ 2 ].setPosition( SCREEN_HEIGHT-BUTTON_HEIGHT,0);
		gButtons[ 3 ].setPosition( 0, (SCREEN_WIDTH-BUTTON_WIDTH*3)/2+BUTTON_WIDTH );
		gButtons[ 4 ].setPosition( (SCREEN_HEIGHT-BUTTON_HEIGHT*3)/2+BUTTON_HEIGHT,(SCREEN_WIDTH-BUTTON_WIDTH*3)/2+BUTTON_WIDTH );
		gButtons[ 5 ].setPosition( SCREEN_HEIGHT-BUTTON_HEIGHT, (SCREEN_WIDTH-BUTTON_WIDTH*3)/2+BUTTON_WIDTH);
		gButtons[ 6 ].setPosition( 0, SCREEN_WIDTH-BUTTON_WIDTH );
		gButtons[ 7 ].setPosition( (SCREEN_HEIGHT-BUTTON_HEIGHT*3)/2+BUTTON_HEIGHT, SCREEN_WIDTH-BUTTON_WIDTH );
		gButtons[ 8 ].setPosition( SCREEN_HEIGHT-BUTTON_HEIGHT, SCREEN_WIDTH-BUTTON_WIDTH );
	}
	SDL_Surface *Draw = IMG_Load("TicTacToeGame/draw.png");
	if(Draw==NULL){
		printf( "Failed to load Draw texture!\n" );
		success = false;
	}
	SDL_Surface *XWin = IMG_Load("TicTacToeGame/XWin.png");
	if(Draw==NULL){
		printf( "Failed to load XWin texture!\n" );
		success = false;
	}
	SDL_Surface *OWin = IMG_Load("TicTacToeGame/OWin.png");
	if(Draw==NULL){
		printf( "Failed to load OWin texture!\n" );
		success = false;
	}

	return success;
}

int checkwin(char* play,int &draw){
	for(int i=0;i<=2;i++){
		if(play[3*i]==play[3*i+1]&&play[3*i+1]==play[3*i+2]){
			draw =i;
			return 1;
		}
	}
	for(int i=0;i<=2;i++){
		if(play[3*i]==play[3*i+3]&&play[3*i]==play[3*i+6]){
			draw=i+3;
			return 1;
		}
	}
	if(play[0]==play[4]&&play[0]==play[8]){
		draw=6;
		return 1;
	}
	if(play[2]==play[4]&&play[2]==play[6]){
		draw=7;
		return 1;
	}
	return 0;
}

void drawLine(int draw){
	
	if(3<=draw<=5){
		SDL_SetRenderDrawColor(gRenderer,0,255,255,0);
		SDL_RenderDrawLine(gRenderer,105+((draw-3)*210),0,105+((draw-3)*210),640);
	}
	if(0<=draw<=2){
		SDL_SetRenderDrawColor(gRenderer,0,255,255,0);
		SDL_RenderDrawLine(gRenderer,0,105+(210*draw),640,105+(210*draw));
	}
	if(draw==6){
		SDL_SetRenderDrawColor(gRenderer,0,255,255,0);
		SDL_RenderDrawLine(gRenderer,5,5,635,635);
	}
	if(draw==7){
		SDL_SetRenderDrawColor(gRenderer,0,255,255,0);
		SDL_RenderDrawLine(gRenderer,635,5,5,635);
	}
}


void close()
{
	//Free loaded images
	gButtonSpriteSheetTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	bool turn =0;
	int endgame = 0;
	int draw=-1;
	int played[9]={0,1,2,3,4,5,6,7,8};
	char play[9]={'a','b','c','d','e','f','g','h','t'};
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			int k=9;
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if(e.type==SDL_QUIT) {
						quit=true;
						goto c;
					}
				}
				while(!endgame){
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if(e.type==SDL_QUIT) {
						quit=true;
						goto c;
					}
					//Handle button events
					for( int i = 0; i < 9; ++i )
					{
						if(played[i]<0) continue;
						gButtons[ played[i] ].handleEvent( &e,turn );
						if(gButtons[ played[i] ].handleEvent( &e,turn )==true){
							if(turn==0) play[i]='X';
							else if(turn==1) play[i]='O';
							played[i]=-1;
							if(checkwin(play,draw)==1) {
								endgame=1;
								goto b;
							}
							k--;
							if(turn==false) turn=true;
							else if(turn==true) turn=false;
							if(k==0) endgame=1;
							
						}
					}
				}
				b:;
				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				SDL_SetRenderDrawColor(gRenderer,0,0,0,0);
				SDL_RenderDrawLine(gRenderer,213,0,213,640);
				SDL_RenderDrawLine(gRenderer,427,0,427,640);
				SDL_RenderDrawLine(gRenderer,0,213,640,213);
				SDL_RenderDrawLine(gRenderer,0,427,640,427);

				//Render buttons
				for( int i = 0; i < TOTAL_BUTTONS; ++i )
				{
					gButtons[ i ].render();
				}
			SDL_Delay(50);
				//Update screen
				if(endgame ==1 ){
					drawLine(draw);
					SDL_RenderPresent(gRenderer);
					SDL_Delay(1000);
				}
				SDL_RenderPresent( gRenderer );
			}
			SDL_DestroyRenderer(gRenderer);
			SDL_Surface *GetEndGame=SDL_GetWindowSurface(gWindow);
			if(k==0) SDL_BlitScaled(Draw,NULL,GetEndGame,NULL);
			else if(k!=0 &&turn == 0) SDL_BlitScaled(XWin,NULL,GetEndGame,NULL);
			else if(k!=0 &&turn == 1) SDL_BlitScaled(OWin,NULL,GetEndGame,NULL);
			SDL_UpdateWindowSurface(gWindow);
			}
			c:;
		}
	}
	//Free resources and close SDL
	close();

	return 0;
}
