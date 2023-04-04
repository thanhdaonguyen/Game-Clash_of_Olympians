//
//  LTexture.cpp
//  Game:Clash_of_Olympians
//
//  Created by Thành Đạo Nguyễn on 20/03/2023.
//

#include "LTexture.hpp"

//Ltexture's functions
LTexture::LTexture(SDL_Renderer*& renderer) {
    //Initialize
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
        printf( "Unable to load image from %s. Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else {
        // Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 255, 255, 255));

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(aRenderer, loadedSurface);
        if( newTexture == NULL) {
            printf("Unable to create texture from surface %s, SDL_Error: %s", path.c_str(), SDL_GetError());
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

int LTexture::getHeigt() {
    return mHeight;
}

