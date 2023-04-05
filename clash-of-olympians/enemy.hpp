//
//  enemy.hpp
//  clash-of-olympians
//
//  Created by Thành Đạo Nguyễn on 04/04/2023.
//

#ifndef enemy_hpp
#define enemy_hpp

#include <stdio.h>
#include "header1.hpp"
#include "LTexture.hpp"

class Enemy {
public:
    //the velocity of the enemy
    double mVelX;
    double mVelY;
    //the damage of the enemy
    int mDam;
    //timer to check if it do damage
    int mTimer = 0;
    //the type of the enemy
    std::string mType;
    
    //Constructor
    Enemy (int x, int y, std::string type);
    //move the enemy
    void move();
    //deal damage to the tower
    int doDamage();
    //show the enemy on
    void render(SDL_Renderer*& renderer, LTexture& emTexture);
    //Get the coordinates
    double getPosX();
    double getPosY();
    //set the stop position
    void setStop(double x);
    //Moves the collision boxes relative to the position
    void shiftColliders();
    //check if the enemy approached close enough or be touched
    bool isStop = false;
    bool isTouched = false;
    //Dot's collision boxes
    std::vector<SDL_Rect> mColliders;
    //clip
    SDL_Rect mClip = {192, 192, 64, 64};

private:
    //Position of the object
    double mPosX, mPosY;
    //The position where the enemy would stop
    double stopX = 350;
    
        
};

#endif /* enemy_hpp */
