//
//  Dot.hpp
//  Game:Clash_of_Olympians
//
//  Created by Thành Đạo Nguyễn on 22/03/2023.
//

#ifndef Dot_hpp
#define Dot_hpp

#include <stdio.h>
#include "header1.hpp"
#include "LTexture.hpp"


class Dot {
public:
    //The dimensions of the dot
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;
    static const int DOT_THROW_VEL = 10;
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    
    //Maximum axis velocity of the dot
    static const int DOT_VEL = 3;
    
    //Initializes the variables
    Dot(int x, int y);
    
    //Takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event& e);
    
    //Moves the dot
    void move();
    
    //Shows the dot on the screen
    void render(SDL_Renderer*& renderer, LTexture& dotTexture);
    
    //Get the collision boxes
    std::vector<SDL_Rect>& getColliders();
    
private:
    //The X and Y offsets of the dot
    double mPosX, mPosY;
    
    //The velocity of the dot
    double mVelX, mVelY;
    
    //Dot's collision boxes
    std::vector<SDL_Rect> mColliders;
    
    //Moves the collision boxes relative to the dot's position
    void shiftColliders();
};


#endif /* Dot_hpp */
