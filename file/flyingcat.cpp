#include"flyingcatlib.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 679;

enum nyanCatFood{
    strawberry,
    cupcake,
    cookie,
    cake,
    peppermint,
    bomb,
    rocket,
    totalFood,
};

SDL_Window *gWindow = NULL;

TTF_Font *gFont=NULL;

SDL_Renderer *gRenderer=NULL;

SDL_Event e;

LTexture backGround;
LTexture catFood[totalFood];
LTexture cat[3];
LTexture rainbow[2];
LTexture FlyingCatStart;
LTexture StartButton;
LTexture Score; 
LTexture BOOM;
LTexture pauseButton;
LTexture pauseBoard;
LTexture continuePlayButton;
LTexture endGameBoard;
LTexture restartButton;
LTexture menuButton;
LTexture GAMEOVER;
LTexture YourHighestScore;

LCat flyingCat;

Mix_Chunk *claim=NULL;
Mix_Chunk *boomSound=NULL;

Mix_Music *startMusic=NULL;
Mix_Music *ingameMusic=NULL;

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
    gFont = TTF_OpenFont( "Game/font.ttf", Font );
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
    if ( gFont!=NULL){
        TTF_CloseFont(gFont);
        gFont=NULL;
    }
}

void LTexture::setColor()
{
    //Modulate texture rgb
    if(checkMouse()){
        SDL_SetTextureColorMod( mTexture, 200, 200, 200 );
    }
    else{
        SDL_SetTextureColorMod( mTexture, 255, 255, 255);
    }
}
		
void LTexture::setAlpha( Uint8 alpha ){
    //Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render(int x, int y,double angel,SDL_Rect* clip, SDL_Point* center, SDL_RendererFlip flip){

    mPosX=x;
    mPosY=y;

    SDL_Rect renderQuad = {x,y,mWidth,mHeight};

    if(clip!=NULL){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad,angel,center,flip);
}

void LTexture::renderMotion(int x,int y,int &motion){
    render(x,y);
    motion++;
    if(motion>=9) motion=0;
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

LCat::LCat()
{
    //Initialize the offsets
    mPosX=30;
    lineToRender=3;
    catMotion=0;
    //Initialize the velocity
    mPosY = mPosY=(SCREEN_HEIGHT/7*lineToRender)-CAT_HEIGHT/2;
    mPosY_ = mPosY=(SCREEN_HEIGHT/7*lineToRender)-CAT_HEIGHT/2;

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

void LCat::render()
{
    //Show the dot
	cat[catMotion/3].render( mPosX, mPosY);
}

bool LCat::isMove(){
    bool isMove=false;
    if(mPosY!=mPosY_) isMove=true;
    return isMove;
}

void LCat::renderMotion(){
    mPosY_=(SCREEN_HEIGHT/7*lineToRender)-CAT_HEIGHT/2;
    if(abs(mPosY_-mPosY)<=mVelY){
        mPosY=mPosY_;
    }
    else{
        if(mPosY<mPosY_) mPosY+=mVelY;
        else if(mPosY>mPosY_) mPosY-=mVelY;
    }
	cat[catMotion/3].renderMotion( mPosX, mPosY,catMotion);
}

bool LCat::check(int mFoodX,int mFoodWidth, int mFoodLine){
    int mCatX=mPosX+CATIMAGE_WIDTH-CAT_WIDTH;
    if(lineToRender!=mFoodLine) return false;
    if(mCatX+CAT_WIDTH<mFoodX) return false;
    if(mCatX>mFoodX+mFoodWidth) return false;
    return true;
}

LFood::LFood(){
    mPosX=-300;
    mPosY=0;
    foodToRender=0;
    lineToRender=0;
}

void LFood::renderMotion(){
    mPosY=(SCREEN_HEIGHT/7*lineToRender)-FOOD_HEIGHT/2;
    catFood[foodToRender].render(mPosX,mPosY);
    if(mPosX>-300) mPosX-=mVelX;
}

void LFood::render(){
    mPosY=(SCREEN_HEIGHT/7*lineToRender)-FOOD_HEIGHT/2;
    catFood[foodToRender].render(mPosX,mPosY);
}

LRainbow::LRainbow(){
    mPosX=0;
    mPosY=flyingCat.mPosY;
    rainbowMotion=0;
    mVelY=1;
}

void LRainbow::render(){
    if(rainbowMotion>=8) rainbowMotion=0;
    rainbow[rainbowMotion/4].render(mPosX,mPosY);
}

void LRainbow::renderMotion(){
    if(rainbowMotion>=8) rainbowMotion=0;
    if(flyingCat.isMove()){
        rainbow[rainbowMotion/4].render(mPosX,mPosY);
    }
    else rainbow[rainbowMotion/4].renderMotion(mPosX,mPosY,rainbowMotion);

    if(mPosY!=flyingCat.mPosY){
        if(abs(mPosY-flyingCat.mPosY)<=mVelY) mPosY=flyingCat.mPosY;
        if(mPosY<flyingCat.mPosY) mPosY+=mVelY;
        else if(mPosY>flyingCat.mPosY) mPosY-=mVelY;
    }
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

    //load background
    if( !backGround.loadFromFile( "Game/nyancatbackground.jpg" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !FlyingCatStart.loadFromFile( "Game/flyingcatstart.png" ) )
	{
		printf( "Failed to render text texture!\n" );
		success = false;
	}

    //load cat
    if( !cat[0].loadFromFile( "Game/nyancat0.png" ) )
	{
		printf( "Failed to load cat texture!\n" );
		success = false;
	}
    if( !cat[1].loadFromFile( "Game/nyancat1.png" ) )
	{
		printf( "Failed to load cat texture!\n" );
		success = false;
	}
    if( !cat[2].loadFromFile( "Game/nyancat2.png" ) )
	{
		printf( "Failed to load cat texture!\n" );
		success = false;
	}

    //load rainbow
    if( !rainbow[0].loadFromFile( "Game/rainbow0.png" ) )
	{
		printf( "Failed to load rainbow texture!\n" );
		success = false;
	}
    if( !rainbow[1].loadFromFile( "Game/rainbow1.png" ) )
	{
		printf( "Failed to load rainbow texture!\n" );
		success = false;
	}

    //load food
    if( !catFood[strawberry].loadFromFile("Game/strawberry.png")){
        printf("Failed to load food texture!\n");
        success=false;
    }
    if( !catFood[cupcake].loadFromFile("Game/cupcake.png")){
        printf("Failed to load food texture!\n");
        success=false;
    }
    if( !catFood[cookie].loadFromFile("Game/cookie.png")){
        printf("Failed to load food texture!\n");
        success=false;
    }
    if( !catFood[cake].loadFromFile("Game/cake.png")){
        printf("Failed to load food texture!\n");
        success=false;
    }
    if( !catFood[peppermint].loadFromFile("Game/peppermint.png")){
        printf("Failed to load food texture!\n");
        success=false;
    }
    if( !catFood[bomb].loadFromFile("Game/bomb.png")){
        printf("Failed to load food texture!\n");
        success=false;
    }
    if( !catFood[rocket].loadFromFile("Game/rocket.png")){
        printf("Failed to load food texture!\n");
        success=false;
    }

    //load Boom!
    if( !BOOM.loadFromFile("Game/boom!.png")){
        printf("Failed to load food texture!\n");
        success=false;
    }

    //load board and button
    if( !endGameBoard.loadFromFile("Game/endgameboard.png")){
        printf("Failed to load end game board texture!\n");
        success=false;
    }
    if( !restartButton.loadFromFile("Game/restartbutton.png")){
        printf("Failed to load restart button texture!\n");
        success=false;
    }
    if( !menuButton.loadFromFile("Game/menubutton.png")){
        printf("Failed to load menu button texture!\n");
        success=false;
    }
    if( !pauseButton.loadFromFile("Game/pausebutton.png")){
        printf("Failed to load pause button texture!\n");
        success=false;
    }
    if( !pauseBoard.loadFromFile("Game/pauseboard.png")){
        printf("Failed to load pause board texture!\n");
        success=false;
    }
    if( !continuePlayButton.loadFromFile("Game/continueplaybutton.png")){
        printf("Failed to load continue play button texture!\n");
        success=false;
    }
    if( !StartButton.loadFromFile( "Game/startbutton.png" ) )
	{
		printf( "Failed to render text texture!\n" );
		success = false;
	}

    //load music
    claim=Mix_LoadWAV("Game/claim.wav");
    if( claim == NULL )
	{
		printf( "Failed to load claim sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
    boomSound=Mix_LoadWAV("Game/boomsound.wav");
    if( boomSound == NULL )
	{
		printf( "Failed to load boom sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
    startMusic=Mix_LoadMUS("Game/startmusic.wav");
    if(startMusic==NULL){
        printf( "Failed to load start sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
    }
    ingameMusic=Mix_LoadMUS("Game/ingamemusic.wav");
    if(ingameMusic==NULL){
        printf( "Failed to load ingame sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
    }

    //Render text
	SDL_Color textColor = { 255,0,0};
    if( !GAMEOVER.loadFromRenderedText("GAME OVER!",textColor, 50)){
        printf("Failed to load Game Over! text texture!\n");
        success=false;
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
    FlyingCatStart.free();
    Score.free();
    StartButton.free();
    endGameBoard.free();
    restartButton.free();
    menuButton.free();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer=NULL;
    gWindow=NULL;

    Mix_FreeChunk( claim );
    Mix_FreeMusic( startMusic );
    Mix_FreeMusic( ingameMusic );

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

//cac bien trong tro choi
int score = 0;
int scrollingOffset=0;
int scrollingOffsetVel=2;
bool spawnFood[3]={false};
bool hadSpawned[3]={false};
int difficultToTime=0;
int difficultVel=0;
int highestScore=0;
bool isHighestScore=false;
bool pauseGame=false;
bool start=false;
bool endGame =false;
bool quit=false;
bool goRestart;
bool goMenu;
LFood food[45];
LRainbow flyingCatRainbow[5];
SDL_Color textColor={255,0,0};
std::string SCORETEXT="";
std::string YourHighestScoreText="";

void setRainbow(){
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

void toMenu(){
    goMenu=true;
    score=0;
    scrollingOffset=0;
    for(int i=0;i<3;i++){
        spawnFood[i]=false;
        hadSpawned[i]=false;
    }
    difficultToTime=0;
    difficultVel=0;
    isHighestScore=false;
    pauseGame=false;
    start=false;
    endGame=false;
    for(int i=0;i<45;i++){
        food[i].mPosX=-300;
    }
    Mix_HaltMusic();
}

void toRestart(){
    goRestart=true;
    score = 0;
    scrollingOffset=0;
    for(int i=0;i<3;i++){
        spawnFood[i]=false;
        hadSpawned[i]=false;
    }
    difficultToTime=0;
    difficultVel=0;
    isHighestScore=false;
    endGame=false;
    pauseGame=false; 
    for(int i=0;i<45;i++){
        food[i].mPosX=-300;
    }
    Mix_HaltMusic();
}

void toQuit(){
    start=true;
    pauseGame=false;
    endGame=true;
    quit=true;
}

//while game ! start
void whileHaveNotStarted(){
    while(SDL_PollEvent(&e)){
        if(e.type==SDL_QUIT){
            toQuit();
        }
        if(e.type==SDL_MOUSEBUTTONUP&&StartButton.checkMouse()){
            start=true;
        }
    } 

    if( Mix_PlayingMusic() == 0 )
	{
		//Play the music
		Mix_PlayMusic( startMusic, -1 );
	}

    FlyingCatStart.render(0,0);
                
    StartButton.setColor();
    StartButton.render(SCREEN_WIDTH/7,SCREEN_HEIGHT/2);

    SDL_RenderPresent(gRenderer);
    SDL_Delay(25);
}

//while game pause
void whilePauseGame(){
    while(SDL_PollEvent(&e)!=0){
        if(e.type==SDL_QUIT){
            toQuit();
        }
        if(e.type==SDL_MOUSEBUTTONUP){
            if(continuePlayButton.checkMouse()){
                pauseGame=false;
            }   
            if(menuButton.checkMouse()){
                toMenu();
            }
            if(restartButton.checkMouse()){
                toRestart();
            }
        }
    }

    if(Mix_PlayingMusic()==0){
        Mix_PlayMusic(ingameMusic,-1);
    }

    SDL_SetRenderDrawColor(gRenderer,255,255,255,255);
    SDL_RenderClear(gRenderer);

    backGround.render(scrollingOffset,0,0.0);
    backGround.render(scrollingOffset+backGround.mWidth,0,0.0);

    SDL_SetRenderDrawColor(gRenderer,255,255,255,255);
    for(int i=1;i<=6;i++){
        SDL_RenderDrawLine(gRenderer,0,SCREEN_HEIGHT/7*i,SCREEN_WIDTH,SCREEN_HEIGHT/7*i);
    }

    for(int i=0;i<45;++i){
        food[i].render();
    }   

    for(int i=0;i<5;i++){
        flyingCatRainbow[i].render();
    }

    flyingCat.render();
                
    pauseBoard.render((SCREEN_WIDTH-pauseBoard.mWidth)/2,(SCREEN_HEIGHT-pauseBoard.mHeight)/2);

    continuePlayButton.setColor();
    continuePlayButton.render((SCREEN_WIDTH-continuePlayButton.mWidth)/2,pauseBoard.mPosY+(pauseBoard.mHeight/20*4));

    restartButton.setColor();
    restartButton.render((SCREEN_WIDTH-restartButton.mWidth)/2,pauseBoard.mPosY+(pauseBoard.mHeight/20*9));

    menuButton.setColor();
    menuButton.render((SCREEN_WIDTH-menuButton.mWidth)/2,pauseBoard.mPosY+(pauseBoard.mHeight/20*14));

    Score.render(0,0,0);

    SDL_RenderPresent(gRenderer);
}

//while game play
void whilePlayGame(){
    while(SDL_PollEvent(&e)!=0){
        if(e.type==SDL_QUIT){
        toQuit();
    }

    if(Mix_PlayingMusic()==0){
        Mix_PlayMusic(ingameMusic,-1);
    }

    if(e.type==SDL_MOUSEBUTTONDOWN&&pauseButton.checkMouse()){
        pauseGame=true;
    }

    flyingCat.handleEvent(e);
    }

    if(difficultToTime<=10000) difficultToTime++;
    difficultVel=difficultToTime/1000;
                    
    scrollingOffset-=(scrollingOffsetVel+difficultVel);

    SDL_SetRenderDrawColor(gRenderer,255,255,255,255);
    SDL_RenderClear(gRenderer);

    //rolling background
    if(scrollingOffset<-backGround.mWidth){
        scrollingOffset=0;
    }
    backGround.render(scrollingOffset,0);
    backGround.render(scrollingOffset+backGround.mWidth,0);

    SDL_SetRenderDrawColor(gRenderer,255,255,255,255);
    for(int i=1;i<=6;i++){
        SDL_RenderDrawLine(gRenderer,0,SCREEN_HEIGHT/7*i,SCREEN_WIDTH,SCREEN_HEIGHT/7*i);
    }

    //spawn the food
    if(spawnFood[0]==true&&hadSpawned[0]==false){
        for(int i=0;i<15;++i){
            food[i].mPosX=SCREEN_WIDTH+90*i;;
            food[i].lineToRender=rand()%6+1;
            food[i].foodToRender=rand()%totalFood;
        }
        hadSpawned[0]=true;
        hadSpawned[1]=false;
        hadSpawned[2]=false;
    }

    if(spawnFood[1]==true&&hadSpawned[1]==false){
        for(int i=15;i<30;++i){
            food[i].mPosX=SCREEN_WIDTH+90*(i-15);;
            food[i].lineToRender=rand()%6+1;
            food[i].foodToRender=rand()%totalFood;
    }
        hadSpawned[0]=false;
        hadSpawned[1]=true;
        hadSpawned[2]=false;
    } 

    if(spawnFood[2]==true&&hadSpawned[2]==false){
        for(int i=30;i<45;++i){
            food[i].mPosX=SCREEN_WIDTH+90*(i-30);;
            food[i].lineToRender=rand()%6+1;
            food[i].foodToRender=rand()%totalFood;
        }
        hadSpawned[0]=false;
        hadSpawned[1]=false;
        hadSpawned[2]=true;
    }

    //render the food
    for(int i=0;i<45;i++){
        if(food[i].foodToRender==bomb) food[i].mVelX=10+difficultVel;
        else if(food[i].foodToRender==rocket) food[i].mVelX=13+difficultVel;
        else food[i].mVelX=7+difficultVel;

        if(food[14].mPosX>SCREEN_WIDTH-2*food[14].FOOD_WIDTH&&food[14].mPosX<SCREEN_WIDTH-food[14].FOOD_WIDTH) spawnFood[1]=true;
        else spawnFood[1]=false;

        if(food[29].mPosX>SCREEN_WIDTH-2*food[29].FOOD_WIDTH&&food[29].mPosX<SCREEN_WIDTH-food[29].FOOD_WIDTH) spawnFood[2]=true;
        else spawnFood[2]=false;

        if(food[44].mPosX>SCREEN_WIDTH-2*food[44].FOOD_WIDTH&&food[44].mPosX<SCREEN_WIDTH-food[44].FOOD_WIDTH) spawnFood[0]=true;
        else spawnFood[0]=false;

        food[i].renderMotion();

        //while eat
        if(flyingCat.check(food[i].mPosX,85,food[i].lineToRender)==true){
            if(food[i].foodToRender>=bomb){
                BOOM.mPosX=food[i].mPosX+food[i].FOOD_WIDTH/2-BOOM.mWidth/2;
                BOOM.mPosY=food[i].mPosY+food[i].FOOD_HEIGHT/2-BOOM.mHeight/2;
                endGame=true;
            }
            else{
                food[i].mPosX=-300;
                score++;
                Mix_PlayChannel(-1,claim,0);
            }
        }
    }

    //render the rainbow
    for(int i=0;i<5;i++){
        flyingCatRainbow[i].renderMotion();
    }

    flyingCat.renderMotion();

    pauseButton.render(SCREEN_WIDTH-pauseButton.mWidth,0);

    Score.free();
    SCORETEXT="SCORE: ";
    SCORETEXT+=toString(score);
    Score.loadFromRenderedText(SCORETEXT.c_str(),textColor,20);

    Score.render(0,0,0);

    SDL_RenderPresent(gRenderer);
}

//check is score higher than highest score
void checkScore(){
    if(score<=highestScore){
        isHighestScore=false;
        Score.free();
        SCORETEXT="YOUR SCORE: ";
        SCORETEXT+=toString(score);
        Score.loadFromRenderedText(SCORETEXT.c_str(),textColor,50);

        YourHighestScore.free();
        YourHighestScoreText="YOUR HIGHEST SCORE: ";
        YourHighestScoreText+=toString(highestScore);
        YourHighestScore.loadFromRenderedText(YourHighestScoreText.c_str(),textColor,50);
    }

    else {
        highestScore=score;
        isHighestScore=true;
        Score.free();
        SCORETEXT="NEW HIGHEST SCORE: ";
        SCORETEXT+=toString(score);
        Score.loadFromRenderedText(SCORETEXT.c_str(),textColor,50);
    }
} 

//while end game
void whileEndGame(int &alpha){
    while(SDL_PollEvent(&e)!=0){
        if(e.type==SDL_QUIT){
        toQuit();
        }

        if(e.type==SDL_MOUSEBUTTONUP){
            if(menuButton.checkMouse()){
                toMenu();
            }

            if(restartButton.checkMouse()){
                toRestart();
            }
        }
    }

    SDL_SetRenderDrawColor(gRenderer,255,255,255,255);
    SDL_RenderClear(gRenderer);

    backGround.render(scrollingOffset,0,0.0);
    backGround.render(scrollingOffset+backGround.mWidth,0,0.0);

    SDL_SetRenderDrawColor(gRenderer,255,255,255,255);
    for(int i=1;i<=6;i++){
        SDL_RenderDrawLine(gRenderer,0,SCREEN_HEIGHT/7*i,SCREEN_WIDTH,SCREEN_HEIGHT/7*i);
    }

    for(int i=0;i<45;++i){
        food[i].render();
    }

    for(int i=0;i<5;i++){
        flyingCatRainbow[i].render();
    }

    flyingCat.render();

    BOOM.render(BOOM.mPosX,BOOM.mPosY);

    endGameBoard.setAlpha(alpha);
    restartButton.setAlpha(alpha);
    menuButton.setAlpha(alpha);

    alpha+=4;
    if(alpha>255) alpha=255;

    endGameBoard.render((SCREEN_WIDTH-endGameBoard.mWidth)/2,(SCREEN_HEIGHT-endGameBoard.mHeight)/2);

    restartButton.setColor();
    restartButton.render((SCREEN_WIDTH-endGameBoard.mWidth)/2+2*restartButton.mWidth,450);

    menuButton.setColor();
    menuButton.render((SCREEN_WIDTH+endGameBoard.mWidth)/2-3*menuButton.mWidth,450);

    GAMEOVER.render((SCREEN_WIDTH-GAMEOVER.mWidth)/2,200);

    if(isHighestScore==true){
        Score.render((SCREEN_WIDTH-Score.mWidth)/2,325);
    }
    else{
        Score.render((SCREEN_WIDTH-Score.mWidth)/2,280);
        YourHighestScore.render((SCREEN_WIDTH-YourHighestScore.mWidth)/2,360);
    }

    SDL_RenderPresent(gRenderer);
    SDL_Delay(25);
}

//main
int main(int argc,char*argv[]){
    
    SDL_DestroyWindow(gWindow);

    if( !init() ){
        printf("Failed to initialize!\n");
    }
    else{
        if(!loadMedia()){
            printf("Failed to load media!\n");
        }
        else{
            //set the rainbow
            setRainbow();

            //if menu
            menu:;

            //start
            while(!start){
                whileHaveNotStarted();
            }

            Mix_HaltMusic();

            //if restart game
            restart:;
            spawnFood[0]=true;

            //play game
            while( !endGame ){
                //if pause game
                while(pauseGame){
                    whilePauseGame();

                    //if click to menu or restart
                    if(goMenu==true){
                        goMenu=false;
                        goto menu;
                    }

                    if(goRestart==true){
                        goRestart=false;
                        goto restart;
                    }
                }

                //while play game
                whilePlayGame();
            }
            Mix_PlayChannel(-1,boomSound,0);

            //end game
            checkScore();

            int alpha=0;

            while(!quit){
                whileEndGame(alpha);
                //if click to menu or restart
                if(goMenu==true){
                    goMenu=false;
                    goto menu;
                }

                if(goRestart==true){
                    goRestart=false;
                    goto restart;
                }
            }
        }
    }

    close();
    return 0;
}
