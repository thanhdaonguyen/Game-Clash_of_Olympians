//
//  Hero.hpp
//  Game:Clash_of_Olympians
//
//  Created by Thành Đạo Nguyễn on 22/03/2023.
//

#ifndef Hero_hpp
#define Hero_hpp
#include "header1.hpp"
#include <iostream>
#include "LTexture.hpp"
class Hero {
    
public:
    
    //Initialize the hero
    Hero(int x, int y);
    
//    //Takes key presses and adjust state
//    void handleEvent(SDL_Event& e);
    
    //Get the heroimage
    void getImage(std::string path);
    
    //Show the hero on the screen
    void render(SDL_Renderer*& renderer, LTexture& heroTexture);
    
private:
    //texture that contain of the hero
    std::string hTexture;
    
    //image's positions
    int hPosX;
    int hPosY;
    
};



#endif /* Hero_hpp */
