//
//  SDLApp.cpp
//  clash-of-olympians
//
//  Created by Thành Đạo Nguyễn on 07/04/2023.
//

#include "SDLApp.hpp"

using namespace std;

//Constructor
SDLApp::SDLApp(int imgFlag, const char* title, int x, int y, int w, int h) {
    
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else {

        //Set texture filtering to linear
        if( !SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf ("Warning: Linear texture filtering not enabled");
        }

        //Create window
        mWindow = SDL_CreateWindow( title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN );
        if( mWindow == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else {
            //create renderer for window
            mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(mRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s/n", SDL_GetError());
            }
            else {
                //Initialize PNG loading
                if (!(IMG_Init(imgFlag) & imgFlag)) {
                    printf("Renderer could not be created! SDL_image Error: %s\n", IMG_GetError());
                }
                
                //Initialize SDL_ttf
                if( TTF_Init() == -1 ) {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                }

            }
        }
    }
}

//Destructor
SDLApp::~SDLApp() {
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
    mWindow = NULL;
    mRenderer = NULL;

    
    //Quit the subsystems
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}

//Get the renderer
SDL_Renderer*& SDLApp::GetRenderer(){
    return mRenderer;
}

//Handle events
void SDLApp::SetEventCallback(function<void()> func) {
    mEventCallback = func;
}

//Handle rendering
void SDLApp::SetRenderCallback(function<void()> func) {
    mRenderCallback = func;
}

//Run app loop
void SDLApp::RunAppLoop() {
    while (!mQuit) {
        //Do event specified by user
        mEventCallback();
        
        //Render to screen specified by user
        mRenderCallback();
        
        //Show what we have drawn on the screen
        SDL_RenderPresent(mRenderer);
    }
}

void SDLApp::EndAppLoop() {
    mQuit = true;
}
