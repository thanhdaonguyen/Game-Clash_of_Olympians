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
    //The max velocity of the dot
    double DOT_THROW_VEL = 12;
    //The real velocity
    double mVel = DOT_THROW_VEL;
    //the screen 
    const int SCREEN_WIDTH = 1200;
    const int SCREEN_HEIGHT = 700;
    //the angle of the spear
    double mAngle;
    //if the mouse is holding
    bool isHolding = false;
    
    //Constructor
    Dot(int x, int y, SDL_Renderer*& renderer, std::string img);
    
    //Destructor
    ~Dot();
    
    //Takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event& e);
    
    //Moves the dot
    void move();
    //change angle
    void changeAngle();
    //Shows the dot on the screen
    void render();
    
    double getPosX();
    
    double getPosY();
    
    bool isTouched = false;

    //Dot's collision boxes
    std::vector<SDL_Rect> mColliders;
    //Get the collision boxes
    std::vector<SDL_Rect>& getColliders();
    //clip
    //clip
    SDL_Rect mClip;
    
private:
    //The X and Y offsets of the dot
    double mPosX, mPosY;
    
    //The x and y input
    double mcPosX, mcPosY;
    
    //The velocity acording to coordinates of the dot
    double mVelX, mVelY;
    
    //Moves the collision boxes relative to the dot's position
    void shiftColliders();
    
    LTexture* mLTexture = NULL;
    SDL_Renderer* mRenderer = NULL;
};


#endif /* Dot_hpp */
