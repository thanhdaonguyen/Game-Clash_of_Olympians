//
//  Hero.cpp
//  Game:Clash_of_Olympians
//
//  Created by Thành Đạo Nguyễn on 22/03/2023.
//

#include "Hero.hpp"

//Hero functions
Hero::Hero(int x, int y) {
    //get the hero's position
    hPosX = x;
    hPosY = y;
}

void Hero::render(SDL_Renderer*& renderer, LTexture& heroTexture) {
    heroTexture.render(renderer, hPosX, hPosY);
    heroTexture.loadFromFile(renderer,hTexture);
};

void Hero::getImage(std::string path) {
    hTexture = path;
}
