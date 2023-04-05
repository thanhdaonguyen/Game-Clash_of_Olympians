//
//  enemy.cpp
//  clash-of-olympians
//
//  Created by Thành Đạo Nguyễn on 04/04/2023.
//

#include "enemy.hpp"

Enemy::Enemy(int x, int y, std::string type) {
    //Initialize the coordinates
    mPosX = x;
    mPosY = y;
    
    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
    
    //Initialize type
    mType = type;
    
    if (type == "Goblin") {
        //Create the necessary SDL_Rects
        mColliders.resize(1);
        //Initialize the collision boxes's width and height
        mColliders[0].w = 64;
        mColliders[0].h = 64;
    }
    
    //Initialize colliders relatives to position
    shiftColliders();
}

void Enemy::move() {
    //Move the object
    mPosX += mVelX;
    mPosY += mVelY;
    //change the colliders' positions
    shiftColliders();
    
    //If the enemy is close enough, it will stop
    if ( mPosX < stopX ) {
        mVelY = 0;
        mVelX = 0;
        isStop = true;
    }
    
}
double Enemy::getPosY() {
    return mPosY;
}

double Enemy::getPosX() {
    return mPosX;
}

void Enemy::setStop(double x) {
    stopX = x;
}

void Enemy::shiftColliders() {
    //the row offset
    int r = 0;
    
    //Go through the object's collision boxes
    for (int set = 0; set > mColliders.size(); set++) {
        mColliders[set].x = mPosX;
        mColliders[set].y = mPosY + r;
        
        //Move the row offset down the height of the collision box
        r += mColliders[set].h;
    }
    
}
