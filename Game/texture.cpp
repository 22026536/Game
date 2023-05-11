#include"flyingcatlib.h"

LTexture::LTexture(){
    mTexture=NULL;
    mWidth=0;
    mHeight=0;
    mPosX=-0;
    mPosY=0;
}

LTexture::~LTexture(){
    free();
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

bool LTexture::loadFromFile( std::string path, SDL_Renderer *gRenderer )
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

bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor, SDL_Renderer *gRenderer, TTF_Font *gFont )
{
	//Get rid of preexisting texture
	free();

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

void LTexture::setColor()
{
    if(checkMouse()){
        SDL_SetTextureColorMod( mTexture, 200, 200, 200 );
    }
    else{
        SDL_SetTextureColorMod( mTexture, 255, 255, 255);
    }
}

void LTexture::setAlpha( Uint8 alpha ){
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render(int x, int y, SDL_Renderer *gRenderer, double angel,SDL_Rect* clip, SDL_Point* center, SDL_RendererFlip flip){

    mPosX=x;
    mPosY=y;

    SDL_Rect renderQuad = {x,y,mWidth,mHeight};

    if(clip!=NULL){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad,angel,center,flip);
}

bool LTexture::checkMouse(){
    bool inside = true;
    int x, y;
	SDL_GetMouseState( &x, &y );

    if(x<mPosX) inside = false;
    else if(x>mPosX+mWidth) inside = false;
    else if(y<mPosY) inside = false;
    else if(y>mPosY+mHeight) inside = false;
    return inside;
}

