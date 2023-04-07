//
//  LTexture.cpp
//  Game:Clash_of_Olympians
//
//  Created by Thành Đạo Nguyễn on 20/03/2023.
//

#include "LTexture.hpp"
using namespace std;
//Ltexture's functions
LTexture::LTexture(SDL_Renderer*& renderer, std::string path) {
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
    loadFromFile(renderer, path);
}

LTexture::LTexture() {
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture() {
    //Deallocate
    free();
}

bool LTexture::loadFromFile(SDL_Renderer*& aRenderer, std:: string path ) {

    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if( loadedSurface == NULL ) {
        if (path != "") {
            printf( "Unable to load image from %s. Error: %s\n", path.c_str(), IMG_GetError() );
            cout << endl;
        }
    }
    else {
        // Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 255, 255, 255));

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(aRenderer, loadedSurface);
        if( newTexture == NULL) {
            printf("Unable to create texture from surface %s, SDL_Error: %s", path.c_str(), SDL_GetError());
            cout << endl;
        }
        else {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight =  loadedSurface->h;
        }

        //Get rid of load loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

bool LTexture::loadFromRenderedText(SDL_Renderer*& renderer, TTF_Font*& gFont, std::string textureText, SDL_Color textColor) {
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        cout << endl;
    }
    else
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
            cout << endl;
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

void LTexture::free() {
    //Free texture if it exists
    if( mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render(SDL_Renderer*& aRenderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight};
    

    //Set clip rendering dimensions
    if(clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
        
    }

    //Render to screen
    SDL_RenderCopyEx(aRenderer, mTexture, clip, &renderQuad, angle, center, flip);
        
}

void LTexture::setColor(Uint8 r, Uint8 g, Uint8 b) {
    //Modulate texture
    SDL_SetTextureColorMod(mTexture, r, g, b);
}

void LTexture::setBlendMod(SDL_BlendMode blending) {
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::blendTexture(Uint8 alpha) {
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
}

