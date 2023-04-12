//
//  Hero.cpp
//  Game:Clash_of_Olympians
//
//  Created by Thành Đạo Nguyễn on 22/03/2023.
//

#include "Hero.hpp"

//Hero functions
Hero::Hero(int x, int y, SDL_Renderer*& renderer, std::string img) {
    //get the hero's position
    hPosX = x;
    hPosY = y;
    
    //Set mLTexture
    mLTexture = new LTexture;
    mLTexture->loadFromFile(renderer, img);
    
    //Set renderer
    mRenderer = renderer;
}
Hero::~Hero() {
    delete mLTexture;
}


void Hero::render() {
    if (isThrow == false) {
        if (isHolding == false) {
            if (mCount / 5 == 0) mLTexture->loadFromFile(mRenderer, "assets/achillies/achillies1.png");
            else if (mCount / 5 == 1) mLTexture->loadFromFile(mRenderer, "assets/achillies/achillies2.png");
            else if (mCount / 5 == 2) mLTexture->loadFromFile(mRenderer, "assets/achillies/achillies3.png");
            else if (mCount / 5 == 3) mLTexture->loadFromFile(mRenderer, "assets/achillies/achillies4.png");
            else if (mCount / 5 == 5) mLTexture->loadFromFile(mRenderer, "assets/achillies/achillies5.png");
            else mCount = 0;
        }
        else {
            if (mCount / 5 == 0) mLTexture->loadFromFile(mRenderer, "assets/achillies/achillies6.png");
            else if (mCount / 5 == 1) mLTexture->loadFromFile(mRenderer, "assets/achillies/achillies7.png");
            else if (mCount / 5 == 2) mLTexture->loadFromFile(mRenderer, "assets/achillies/achillies8.png");
            else if (mCount / 5 == 3) mLTexture->loadFromFile(mRenderer, "assets/achillies/achillies9.png");
            else if (mCount / 5 == 5) mLTexture->loadFromFile(mRenderer, "assets/achillies/achillies10.png");
            else mCount = 0;
        }
    }
    else {
        if (mCount / 5 == 0) mLTexture->loadFromFile(mRenderer, "assets/achillies/achillies11.png");
        else if (mCount / 5 == 1) mLTexture->loadFromFile(mRenderer, "assets/achillies/achillies12.png");
        else if (mCount / 5 == 2) mLTexture->loadFromFile(mRenderer, "assets/achillies/achillies13.png");
        else if (mCount / 5 == 3) mLTexture->loadFromFile(mRenderer, "assets/achillies/achillies14.png");
        else if (mCount / 5 == 4) mLTexture->loadFromFile(mRenderer, "assets/achillies/achillies15.png");
        else if (mCount / 5 == 5) mLTexture->loadFromFile(mRenderer, "assets/achillies/achillies16.png");
        else if (mCount / 5 == 6) mLTexture->loadFromFile(mRenderer, "assets/achillies/achillies17.png");
        else {
            mCount = 0;
            isThrow = false;
            isHolding = false;
        }
    }
    mCount ++;
    mLTexture->render(mRenderer, hPosX, hPosY, NULL, 0.5);
};

