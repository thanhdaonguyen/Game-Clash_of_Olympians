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
    mLTexture->render(mRenderer, hPosX, hPosY);
};

