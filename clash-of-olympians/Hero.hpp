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
    
    //counting
    int mCount = 0;
    //holding
    bool isHolding = false;
    //throwing
    bool isThrow = false;
    //the strength of the hero
    double strength = 3;
    
    
    //Constructor
    
    Hero(int x, int y, SDL_Renderer*& renderer, std::string img);
    //Destructor
    ~Hero();
//    //Takes key presses and adjust state
//    void handleEvent(SDL_Event& e);
    
    //Show the hero on the screen
    void render();
    
    
    
private:
    
    
    //image's positions
    int hPosX;
    int hPosY;
    
    LTexture* mLTexture = NULL;
    SDL_Renderer* mRenderer = NULL;
};



#endif /* Hero_hpp */
