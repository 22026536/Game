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

SDL_Window *gWindow = NULL;

TTF_Font *gFont=NULL;

SDL_Renderer *gRenderer=NULL;

SDL_Event e;

//back ground
LTexture backGround;

//food
LTexture catFood[totalFood];

//cat
LTexture cat[3];

//rainbow
LTexture rainbow[2];

//start back ground
LTexture FlyingCatStart;

//start button
LTexture StartButton;

//Score
LTexture Score; 

//boom!
LTexture BOOM;

//pause button
LTexture pauseButton;

//pause board
LTexture pauseBoard;

//continue play button
LTexture continuePlayButton;

//end game board
LTexture endGameBoard;

//restart button
LTexture restartButton;

//menu button
LTexture menuButton;

//game over
LTexture GameOver;

//highest score
LTexture YourHighestScore;

//cat
LCat flyingCat;

//food
LFood food[30];

//rainbow
LRainbow flyingCatRainbow[5];

//color text
SDL_Color textColor={255,0,0};

//claim and boom! sound
Mix_Chunk *claim = NULL;
Mix_Chunk *boomSound = NULL;

//start music and playing music
Mix_Music *startMusic = NULL;
Mix_Music *ingameMusic = NULL;

//các hàm render ảnh để trong hàm main sẽ dễ hơn
void LCat::render(SDL_Renderer *gRenderer)
{
	cat[catMotion / 3].render( mPosX, mPosY, gRenderer );
}

void LCat::renderMotion(){
    mPosY_ = ( SCREEN_HEIGHT / 7 * lineToRender ) - CAT_HEIGHT / 2;

    if(abs( mPosY_ - mPosY ) <= mVelY){
        mPosY = mPosY_;
    }
    else{
        if(mPosY < mPosY_) mPosY += mVelY;
        else if(mPosY > mPosY_) mPosY -= mVelY;
    }

    catMotion++;
    if(catMotion >= 9) catMotion = 0;

	cat[catMotion / 3].render( mPosX, mPosY, gRenderer );
}

void LFood::render(){
    mPosY = ( SCREEN_HEIGHT / 7 * lineToRender ) - FOOD_HEIGHT / 2;

    catFood[foodToRender].render( mPosX, mPosY, gRenderer );
}

void LFood::renderMotion(SDL_Renderer *gRenderer){
    catFood[foodToRender].render( mPosX, mPosY, gRenderer );

    if(mPosX >= -FOOD_WIDTH) mPosX -= mVelX;
    else {
        mPosX = -FOOD_WIDTH*2 + totalRender * foodDistance;
        choosed = false;
    }
}


void LRainbow::render(){
    rainbow[rainbowMotion/4].render( mPosX, mPosY, gRenderer );
}

void LRainbow::renderMotion(){
    rainbowMotion++;

    if(rainbowMotion >= 8) rainbowMotion = 0;

    rainbow[rainbowMotion / 4].render( mPosX, mPosY, gRenderer );

    if(mPosY != flyingCat.mPosY){
        if(abs(mPosY-flyingCat.mPosY) <= mVelY) mPosY = flyingCat.mPosY;
        if(mPosY < flyingCat.mPosY) mPosY += mVelY;
        else if(mPosY > flyingCat.mPosY) mPosY -= mVelY;
    }
}

bool loadMedia(){
    bool success = true;

    //load background
    if( !backGround.loadFromFile( "fileImageAndSound/nyancatbackground.jpg", gRenderer) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !FlyingCatStart.loadFromFile( "fileImageAndSound/flyingcatstart.png", gRenderer ) )
	{
		printf( "Failed to render text texture!\n" );
		success = false;
	}

    //load cat
    if( !cat[0].loadFromFile( "fileImageAndSound/nyancat0.png", gRenderer ) )
	{
		printf( "Failed to load cat texture!\n" );
		success = false;
	}
    if( !cat[1].loadFromFile( "fileImageAndSound/nyancat1.png", gRenderer ) )
	{
		printf( "Failed to load cat texture!\n" );
		success = false;
	}
    if( !cat[2].loadFromFile( "fileImageAndSound/nyancat2.png", gRenderer ) )
	{
		printf( "Failed to load cat texture!\n" );
		success = false;
	}

    //load rainbow
    if( !rainbow[0].loadFromFile( "fileImageAndSound/rainbow0.png", gRenderer ) )
	{
		printf( "Failed to load rainbow texture!\n" );
		success = false;
	}
    if( !rainbow[1].loadFromFile( "fileImageAndSound/rainbow1.png", gRenderer ) )
	{
		printf( "Failed to load rainbow texture!\n" );
		success = false;
	}

    //load food
    if( !catFood[strawberry].loadFromFile("fileImageAndSound/strawberry.png", gRenderer)){
        printf("Failed to load food texture!\n");
        success = false;
    }
    if( !catFood[cupcake].loadFromFile("fileImageAndSound/cupcake.png", gRenderer)){
        printf("Failed to load food texture!\n");
        success = false;
    }
    if( !catFood[cookie].loadFromFile("fileImageAndSound/cookie.png", gRenderer)){
        printf("Failed to load food texture!\n");
        success = false;
    }
    if( !catFood[cake].loadFromFile("fileImageAndSound/cake.png", gRenderer)){
        printf("Failed to load food texture!\n");
        success = false;
    }
    if( !catFood[peppermint].loadFromFile("fileImageAndSound/peppermint.png", gRenderer)){
        printf("Failed to load food texture!\n");
        success = false;
    }
    if( !catFood[gumdrop].loadFromFile("fileImageAndSound/gumdrop.png", gRenderer)){
        printf("Failed to load food texture!\n");
        success = false;
    }
    if( !catFood[bomb].loadFromFile("fileImageAndSound/bomb.png", gRenderer)){
        printf("Failed to load food texture!\n");
        success = false;
    }
    if( !catFood[rocket].loadFromFile("fileImageAndSound/rocket.png", gRenderer)){
        printf("Failed to load food texture!\n");
        success = false;
    }

    //load Boom!
    if( !BOOM.loadFromFile("fileImageAndSound/boom!.png", gRenderer)){
        printf("Failed to load food texture!\n");
        success=false;
    }

    //load board and button
    if( !endGameBoard.loadFromFile("fileImageAndSound/endgameboard.png", gRenderer)){
        printf("Failed to load end game board texture!\n");
        success = false;
    }
    if( !restartButton.loadFromFile("fileImageAndSound/restartbutton.png", gRenderer)){
        printf("Failed to load restart button texture!\n");
        success = false;
    }
    if( !menuButton.loadFromFile("fileImageAndSound/menubutton.png", gRenderer)){
        printf("Failed to load menu button texture!\n");
        success = false;
    }
    if( !pauseButton.loadFromFile("fileImageAndSound/pausebutton.png", gRenderer)){
        printf("Failed to load pause button texture!\n");
        success = false;
    }
    if( !pauseBoard.loadFromFile("fileImageAndSound/pauseboard.png", gRenderer)){
        printf("Failed to load pause board texture!\n");
        success = false;
    }
    if( !continuePlayButton.loadFromFile("fileImageAndSound/continueplaybutton.png", gRenderer)){
        printf("Failed to load continue play button texture!\n");
        success = false;
    }
    if( !StartButton.loadFromFile( "fileImageAndSound/startbutton.png", gRenderer ) )
	{
		printf( "Failed to render text texture!\n" );
		success = false;
	}

    //load music
    claim = Mix_LoadWAV("fileImageAndSound/claim.wav");
    if( claim == NULL )
	{
		printf( "Failed to load claim sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
    boomSound=Mix_LoadWAV("fileImageAndSound/boomsound.wav");
    if( boomSound == NULL )
	{
		printf( "Failed to load boom sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
    startMusic=Mix_LoadMUS("fileImageAndSound/startmusic.wav");
    if(startMusic == NULL){
        printf( "Failed to load start sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
    }
    ingameMusic=Mix_LoadMUS("fileImageAndSound/ingamemusic.wav");
    if(ingameMusic == NULL){
        printf( "Failed to load ingame sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
    }

    //load font
    gFont=TTF_OpenFont("fileImageAndSound/font.ttf",40);

    //Render text
	SDL_Color textColor = {255,0,0};
    if( !GameOver.loadFromRenderedText("GAME OVER!",textColor, gRenderer, gFont)){
        printf("Failed to load Game Over! text texture!\n");
        success = false;
    }

    return success;
}

void close(){
    //free background
    backGround.free();
    
    //free cat
    cat[0].free();
    cat[1].free();
    cat[2].free();

    //free food
    for(int i=0;i<totalFood;i++){
        catFood[i].free();
    }

    //free start background
    FlyingCatStart.free();

    //free score
    Score.free();

    //free start button image
    StartButton.free();

    //free end game board image
    endGameBoard.free();

    //free restart button image
    restartButton.free();

    //free menu button image
    menuButton.free();

    //destroy genderer and window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gRenderer=NULL;
    gWindow=NULL;

    //free mix and music
    Mix_FreeChunk( claim );
    Mix_FreeChunk( boomSound );
    Mix_FreeMusic( startMusic );
    Mix_FreeMusic( ingameMusic );

    //close font
    TTF_CloseFont( gFont );
    gFont=NULL;

    //quit
    IMG_Quit();
    SDL_Quit();
    Mix_Quit();
    TTF_Quit();
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

//cac bien trong tro choi
int score = 0;

int scrollingOffset = 0;
const int scrollingOffsetVel = 2;

int difficultToTime= 0 ;
int difficultVel = 0 ;

int highestScore = 0;
bool isHighestScore = false;

bool pauseGame = false;
bool start = false;
bool endGame = false;
bool quit = false;

bool goRestart = false;
bool goMenu = false;

std::string SCORETEXT = "";
std::string YourHighestScoreText = "";

void toMenu(){
    goMenu = true;

    score = 0;

    scrollingOffset = 0;

    difficultToTime = 0;
    difficultVel = 0;

    isHighestScore = false;
    pauseGame = false;
    start = false;
    endGame = false;

    Mix_HaltMusic();
}

void toRestart(){
    goRestart = true;

    score = 0;

    scrollingOffset = 0;

    difficultToTime = 0;
    difficultVel = 0;

    isHighestScore = false;

    endGame = false;
    pauseGame = false;

    Mix_HaltMusic();
}

void toQuit(){
    start = true;
    pauseGame = false;
    endGame = true;
    quit = true;
}

//while game ! start
void whileHaveNotStarted(){
    while(SDL_PollEvent(&e)){
        if(e.type == SDL_QUIT){
            toQuit();
        }
        if(e.type == SDL_MOUSEBUTTONUP&&StartButton.checkMouse()){
            start = true;
        }
    } 

    if( Mix_PlayingMusic() == 0 )
	{
		//Play the music
		Mix_PlayMusic( startMusic, -1 );
	}

    FlyingCatStart.render( 0,0,gRenderer );
                
    StartButton.setColor();
    StartButton.render( SCREEN_WIDTH/7,SCREEN_HEIGHT/2,gRenderer );

    SDL_RenderPresent( gRenderer );
    SDL_Delay(25);
}

//while game pause
void whilePauseGame(){
    while(SDL_PollEvent( &e ) != 0){
        if(e.type == SDL_QUIT){
            toQuit();
        }
        if(e.type == SDL_MOUSEBUTTONUP){
            if(continuePlayButton.checkMouse()){
                pauseGame = false;
            }   
            if(menuButton.checkMouse()){
                toMenu();
            }
            if(restartButton.checkMouse()){
                toRestart();
            }
        }
    }

    if(Mix_PlayingMusic() == 0){
        Mix_PlayMusic( ingameMusic,-1 );
    }

    SDL_SetRenderDrawColor( gRenderer,255,255,255,255 );
    SDL_RenderClear( gRenderer );

    backGround.render( scrollingOffset,0,gRenderer );
    backGround.render( scrollingOffset+backGround.mWidth,0,gRenderer );

    SDL_SetRenderDrawColor( gRenderer,255,255,255,255 );
    for(int i = 1; i <= 6; i++){
        SDL_RenderDrawLine( gRenderer,0,SCREEN_HEIGHT/7*i,SCREEN_WIDTH,SCREEN_HEIGHT/7*i );
    }

    for(int i = 0; i < 30; i++){
        food[i].render();
    }   

    for(int i = 0; i < 5; i++){
        flyingCatRainbow[i].render();
    }

    flyingCat.render( gRenderer );
                
    pauseBoard.render( ( SCREEN_WIDTH-pauseBoard.mWidth ) / 2, ( SCREEN_HEIGHT-pauseBoard.mHeight ) / 2, gRenderer );

    continuePlayButton.setColor();
    continuePlayButton.render( ( SCREEN_WIDTH-continuePlayButton.mWidth ) / 2, pauseBoard.mPosY + ( pauseBoard.mHeight / 20 * 4 ), gRenderer);

    restartButton.setColor();
    restartButton.render( ( SCREEN_WIDTH-restartButton.mWidth ) / 2, pauseBoard.mPosY + ( pauseBoard.mHeight / 20 * 9 ), gRenderer);

    menuButton.setColor();
    menuButton.render( ( SCREEN_WIDTH-menuButton.mWidth ) / 2, pauseBoard.mPosY + ( pauseBoard.mHeight / 20 * 14 ), gRenderer );

    Score.render( 0, 0, gRenderer );

    SDL_RenderPresent( gRenderer );
}

//while game play
void whilePlayGame(){
    while(SDL_PollEvent( &e ) != 0){
        if(e.type == SDL_QUIT){
            toQuit();
        }

        if(Mix_PlayingMusic() == 0){
            Mix_PlayMusic( ingameMusic,-1 );
        }

        if(e.type == SDL_MOUSEBUTTONDOWN&&pauseButton.checkMouse()){
            pauseGame = true;
        }

        flyingCat.handleEvent( e );
    }

    //set difficult to time played
    if( difficultToTime <= 10000 ) difficultToTime++;
    difficultVel = difficultToTime / 1000;
                    
    scrollingOffset -= (scrollingOffsetVel+difficultVel);

    SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
    SDL_RenderClear( gRenderer );

    //rolling background
    if( scrollingOffset <- backGround.mWidth ){
        scrollingOffset = 0;
    }
    backGround.render( scrollingOffset, 0, gRenderer );
    backGround.render( scrollingOffset + backGround.mWidth, 0, gRenderer );

    SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
    for(int i = 1; i <= 6; i++){
        SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 7 * i, SCREEN_WIDTH, SCREEN_HEIGHT / 7 * i );
    }

    //spawn food
    for(int i = 0; i < 30; i++){
        food[i].chooseFood( difficultVel );

        food[i].renderMotion( gRenderer );

        //while eat
        if(flyingCat.check( food[i].mPosX, food[i].FOOD_WIDTH, food[i].lineToRender) == true){
            if(food[i].foodToRender >= bomb){
                BOOM.mPosX = food[i].mPosX + food[i].FOOD_WIDTH / 2 - BOOM.mWidth / 2;
                BOOM.mPosY = food[i].mPosY + food[i].FOOD_HEIGHT / 2 - BOOM.mHeight / 2;
                endGame = true;
            }
            else{
                food[i].mPosX =- food[i].FOOD_WIDTH;
                score ++;
                Mix_PlayChannel( -1,claim,0 );
            }
        }
    }

    //render the rainbow
    for(int i = 0; i < 5; i++){
        flyingCatRainbow[i].renderMotion();
    }

    flyingCat.renderMotion();

    pauseButton.render( SCREEN_WIDTH - pauseButton.mWidth, 0, gRenderer);

    Score.free();
    SCORETEXT = "SCORE: ";
    SCORETEXT += toString( score );
    Score.loadFromRenderedText( SCORETEXT.c_str(), textColor, gRenderer, gFont );

    Score.render( 0, 0, gRenderer );

    SDL_RenderPresent( gRenderer );
}

//check is score higher than highest score
void checkScore(){
    if(score <= highestScore){
        isHighestScore = false;
        Score.free();
        SCORETEXT = "YOUR SCORE: ";
        SCORETEXT += toString( score );
        Score.loadFromRenderedText( SCORETEXT.c_str(), textColor, gRenderer, gFont );

        YourHighestScore.free();
        YourHighestScoreText="YOUR HIGHEST SCORE: ";
        YourHighestScoreText+=toString( highestScore );
        YourHighestScore.loadFromRenderedText( YourHighestScoreText.c_str(), textColor, gRenderer, gFont );
    }

    else {
        highestScore = score;
        isHighestScore = true;
        Score.free();
        SCORETEXT = "NEW HIGHEST SCORE: ";
        SCORETEXT += toString(score);
        Score.loadFromRenderedText( SCORETEXT.c_str(), textColor, gRenderer, gFont );
    }
} 

//while end game
void whileEndGame(int &alpha){
    while(SDL_PollEvent( &e )!=0){
        if(e.type == SDL_QUIT){
        toQuit();
        }

        if(e.type == SDL_MOUSEBUTTONUP){
            if(menuButton.checkMouse()){
                toMenu();
            }

            if(restartButton.checkMouse()){
                toRestart();
            }
        }
    }

    SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
    SDL_RenderClear( gRenderer );

    backGround.render( scrollingOffset, 0, gRenderer );
    backGround.render( scrollingOffset+backGround.mWidth, 0, gRenderer );

    SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
    for(int i = 1; i <= 6; i++){
        SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 7 * i, SCREEN_WIDTH, SCREEN_HEIGHT / 7 * i );
    }

    for(int i = 0; i < 30; i++){
        food[i].render();
    }

    for(int i = 0; i<5; i++){
        flyingCatRainbow[i].render();
    }

    flyingCat.render( gRenderer );

    BOOM.render( BOOM.mPosX, BOOM.mPosY, gRenderer );

    endGameBoard.setAlpha( alpha );
    restartButton.setAlpha( alpha );
    menuButton.setAlpha( alpha );

    alpha += 4;
    if(alpha > 255) alpha = 255;

    endGameBoard.render( ( SCREEN_WIDTH-endGameBoard.mWidth ) / 2, ( SCREEN_HEIGHT - endGameBoard.mHeight ) / 2, gRenderer );

    restartButton.setColor();
    restartButton.render( ( SCREEN_WIDTH - endGameBoard.mWidth ) / 2 + 2 * restartButton.mWidth, 450, gRenderer );

    menuButton.setColor();
    menuButton.render( ( SCREEN_WIDTH + endGameBoard.mWidth ) / 2 - 3 * menuButton.mWidth, 450, gRenderer );

    GameOver.render( ( SCREEN_WIDTH - GameOver.mWidth ) / 2, 200, gRenderer );

    if(isHighestScore == true){
        Score.render( ( SCREEN_WIDTH - Score.mWidth ) / 2, 325, gRenderer);
    }
    else{
        Score.render( ( SCREEN_WIDTH - Score.mWidth) / 2, 280, gRenderer);
        YourHighestScore.render( ( SCREEN_WIDTH - YourHighestScore.mWidth) / 2, 360, gRenderer );
    }

    SDL_RenderPresent( gRenderer );
    SDL_Delay( 25 );
}

//main
int main(int argc,char*argv[]){
    if( !init() ){
        printf("Failed to initialize!\n");
    }
    else{
        if( !loadMedia() ){
            printf("Failed to load media!\n");
        }
        else{
            //set the rainbow
            setRainbow( flyingCatRainbow );

            //if menu
            menu:;

            //start
            while( !start ){
                whileHaveNotStarted();
            }

            Mix_HaltMusic();

            //if restart game
            restart:;

            for(int i = 0; i<30; i++){
                food[i].setFood( i );
            }
            //play game
            while( !endGame ){
                //if pause game
                while( pauseGame ){
                    whilePauseGame();

                    //if click to menu or restart
                    if( goMenu == true ){
                        goMenu = false;
                        goto menu;
                    }

                    if( goRestart == true ){
                        goRestart = false;
                        goto restart;
                    }
                }

                //while play game
                whilePlayGame();
            }
            Mix_PlayChannel( -1,boomSound,0 );

            //end game
            checkScore();

            int alpha = 0;

            while(!quit){
                whileEndGame( alpha );
                //if click to menu or restart
                if( goMenu == true ){
                    goMenu = false;
                    goto menu;
                }

                if( goRestart == true ){
                    goRestart = false;
                    goto restart;
                }
            }
        }
    }

    close();
    return 0;
}
