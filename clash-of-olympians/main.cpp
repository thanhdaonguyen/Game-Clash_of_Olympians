

#include "header1.hpp"
#include "Dot.hpp"
#include "LTexture.hpp"
#include "Hero.hpp"


using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 700;


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
LTexture gBG(gRenderer);
LTexture gGoblin(gRenderer);



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
    
    if(!gDotTexture.loadFromFile(gRenderer,"assets/26_motion/dot.bmp")) {
        printf ("Failed to load dot image");
        success = false;
    }
    
    if(!gBG.loadFromFile(gRenderer,"assets/background.png")) {
        printf ("Failed to load background image");
        success = false;
    }
    
    if(!gGoblin.loadFromFile(gRenderer,"assets/goblin/goblinsword.png")) {
        printf ("Failed to load gobin image");
        success = false;
    }
    
    
    return success;
}

void close() {

    //Free loaded images
    gDotTexture.free();
    gHero.free();
    gBG.free();
    gGoblin.free();

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
            
            //the dots that will be moving on the screen
            vector<Dot*> dotVec;
            
            //The hero
            Hero myHero(230,450);
            myHero.getImage("assets/Shieldmaiden/4x/idle_1.png");
            
            //The force bar
            SDL_Rect forcebarbound ={268, 428, 64, 14};
            SDL_Rect forcebar = {270, 430, 0, 10};
            bool forceHold = false;
            
            //clip for goblin
            SDL_Rect clip1 = {0, 192, 64, 64};
            int clipcount = 0;
            
            //While application is running
            while( !quit ) {
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 ) {
                    //User requests quit
                    if( e.key.keysym.sym == SDLK_ESCAPE ) {
                        quit = true;
                    }
                    if( e.type == SDL_MOUSEBUTTONDOWN) {
                        dotVec.push_back(new Dot(320,470));
                        forceHold = true;
                    }
                    
                    if( e.type == SDL_MOUSEBUTTONUP) {
                        forceHold = false;
                        dotVec.back()->mVel = dotVec.back()->DOT_THROW_VEL*double(forcebar.w + 9)/60;
                        forcebar.w = 0;
                    }
                    
                    if (dotVec.size() != 0) (dotVec.back())->handleEvent(e);
                    
                    

                }
                // force bar
                if (forceHold && forcebar.w < 60) forcebar.w += 2;
                
                // move dots
                int k = (int)dotVec.size();
                for (int i = k - 1; i >= 0; i--) {
                    if (dotVec[i]->isTouched) {
                        delete dotVec[i];
                        dotVec.erase(dotVec.begin() + i);
                        dotVec.shrink_to_fit();
                    }
                }
                for (int i = 0; i < dotVec.size(); i++) {
                    dotVec[i]->move();
                }
                
                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);
                
                //render dots
                SDL_RenderCopy(gRenderer, gBG.mTexture, NULL, NULL);
                unsigned long k1 = 0, k2 = dotVec.size();
                cout << k2 << endl;
                for(unsigned long i = k1; i < k2; i++) {
                    dotVec[i]->render(gRenderer, gDotTexture);
                }
                //render hero
                myHero.render(gRenderer, gHero);
                
                //render force bar
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0); // set color to blue
                SDL_RenderFillRect(gRenderer, &forcebarbound); // draw filled rectangle
                SDL_SetRenderDrawColor(gRenderer, 100, 200, 40, 0); // set color to blue
                SDL_RenderFillRect(gRenderer, &forcebar); // draw filled rectangle
                
                //render enemy
                gGoblin.render(gRenderer, 800, 600, &clip1);
                clipcount ++;
                if (clipcount %4 == 0) {
                    clip1.x += 64;
                    clipcount = 1;
                }
                if (clip1.x >= 704) clip1.x = 0;
                
                
                //Present
                SDL_RenderPresent(gRenderer);
                
                
            }
        }
    }
    

    //Free resources and close SDL
    close();

    return 0;
}



