

#include "header1.hpp"
#include "Dot.hpp"
#include "LTexture.hpp"
#include "Hero.hpp"


using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Texture wrapper class


//The dot that will move around on the screen


//The hero


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gDotTexture(gRenderer);
LTexture gHero(gRenderer);







bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else {

        //Set texture filtering to linear
        if( !SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf ("Warning: Linear texture filtering not enabled");
        }

        //Create window
        gWindow = SDL_CreateWindow( "Clash of Olympians", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else {
            //create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s/n", SDL_GetError());
                success = false;
            }
            else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("Renderer could not be created! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

bool loadMedia() {
    //Loading success flag
    bool success = true;
    
    if(!gDotTexture.loadFromFile(gRenderer,"26_motion/dot.bmp")) {
        printf ("Failed to load dot image");
        success = false;
    }
    
    return success;
}

void close() {

    //Free loaded images
    gDotTexture.free();
    gHero.free();

    //destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}




int main( int argc, char* args[] ) {


    //Start up SDL and create window
    if( !init() ) {
        printf( "Failed to initialize!\n" );
    }
    else {
        //Load media
        if( !loadMedia() ) {
            printf( "Failed to load media!\n" );
        }
        else {
            //Main loop flag
            bool quit = false;
            
            //Event handler
            SDL_Event e;
            
            //the dot that will be moving on the screen
            Dot dot(150,100);
            
            //The hero
            Hero myHero(100,100);
            myHero.getImage("Shieldmaiden/2x/idle_1.png");
            
            
            //While application is running
            while( !quit ) {
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 ) {
                    //User requests quit
                    if( e.key.keysym.sym == SDLK_ESCAPE ) {
                        quit = true;
                    }
                    dot.handleEvent(e);
                }
                
                //move the dot
                dot.move();
                
                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);
                
                //render objects
                dot.render(gRenderer, gDotTexture);
                myHero.render(gRenderer, gHero);
                
                //Present
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    

    //Free resources and close SDL
    close();

    return 0;
}



