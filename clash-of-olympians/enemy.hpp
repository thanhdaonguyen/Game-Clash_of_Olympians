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

class Enemy {
public:
    //the velocity of the enemy
    double mVelX;
    double mVelY;
    //the damage of the enemy
    double dam;
    //the type of the enemy
    std::string mType;
    
    //Constructor
    Enemy (int x, int y, std::string type);
    //move the enemy
    void move();
    //show the enemy on
    void render();
    //Get the coordinates
    double getPosX();
    double getPosY();
    //set the stop position
    void setStop(double x);
    //Moves the collision boxes relative to the position
    void shiftColliders();
    //check if the enemy approached close enough
    bool isStop = false;
    
    
private:
    //Position of the object
    double mPosX, mPosY;
    //The position where the enemy would stop
    double stopX;
    
    //Dot's collision boxes
    std::vector<SDL_Rect> mColliders;
};

#endif /* enemy_hpp */
